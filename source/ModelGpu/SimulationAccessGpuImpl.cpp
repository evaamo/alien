#include <sstream>
#include <QImage>

#include "ModelBasic/SpaceProperties.h"

#include "CudaWorker.h"
#include "CudaController.h"
#include "SimulationContextGpuImpl.h"
#include "SimulationAccessGpuImpl.h"
#include "SimulationControllerGpu.h"
#include "CudaJobs.h"
#include "DataConverter.h"

namespace
{
	const string SimulationAccessGpuId = "SimulationAccessGpuId";
}

SimulationAccessGpuImpl::SimulationAccessGpuImpl(QObject* parent /*= nullptr*/)
	: SimulationAccessGpu(parent)
{
}

SimulationAccessGpuImpl::~SimulationAccessGpuImpl()
{
}

void SimulationAccessGpuImpl::init(SimulationControllerGpu* controller)
{
    auto modelGpuData = ModelGpuData(controller->getContext()->getSpecificData());
    _cudaConstants = modelGpuData.getCudaConstants();
    _dataTOCache = boost::make_shared<_DataTOCache>(_cudaConstants);
	_context = static_cast<SimulationContextGpuImpl*>(controller->getContext());
	_numberGen = _context->getNumberGenerator();
	auto worker = _context->getCudaController()->getCudaWorker();
	auto size = _context->getSpaceProperties()->getSize();
	_lastDataRect = { { 0,0 }, size };
	for (auto const& connection : _connections) {
		QObject::disconnect(connection);
	}
	_connections.push_back(connect(worker, &CudaWorker::jobsFinished, this, &SimulationAccessGpuImpl::jobsFinished, Qt::QueuedConnection));
}

void SimulationAccessGpuImpl::clear()
{
    auto job = boost::make_shared<_ClearDataJob>(getObjectId());
    scheduleJob(job);
}

void SimulationAccessGpuImpl::updateData(DataChangeDescription const& updateDesc)
{
	auto cudaWorker = _context->getCudaController()->getCudaWorker();

	//heuristic for determining rect
	auto size = _context->getSpaceProperties()->getSize();
	IntRect rect = cudaWorker->isSimulationRunning() ? IntRect{ { 0, 0 }, size } : _lastDataRect;

	auto updateDescCorrected = updateDesc;
	metricCorrection(updateDescCorrected);

	auto job = boost::make_shared<_GetDataForUpdateJob>(getObjectId(), _lastDataRect, _dataTOCache->getDataTO(), updateDescCorrected);
    scheduleJob(job);
    _updateInProgress = true;
}

void SimulationAccessGpuImpl::requireData(ResolveDescription const & resolveDesc)
{
    auto const space = _context->getSpaceProperties();
    requireData(IntRect{ {0, 0}, space->getSize() }, resolveDesc);
}

void SimulationAccessGpuImpl::requireData(IntRect rect, ResolveDescription const & resolveDesc)
{
	auto job = boost::make_shared<_GetDataForEditJob>(getObjectId(), rect, _dataTOCache->getDataTO());
    scheduleJob(job);
}

void SimulationAccessGpuImpl::requireImage(IntRect rect, QImagePtr const& target, std::mutex& mutex)
{
	auto job = boost::make_shared<_GetImageJob>(getObjectId(), rect, target, mutex);
    scheduleJob(job);
}

void SimulationAccessGpuImpl::applyAction(PhysicalAction const & action)
{
    auto job = boost::make_shared<_PhysicalActionJob>(getObjectId(), action);
    scheduleJob(job);
}

DataDescription const & SimulationAccessGpuImpl::retrieveData()
{
	return _dataCollected;
}

void SimulationAccessGpuImpl::scheduleJob(CudaJob const & job)
{
    auto worker = _context->getCudaController()->getCudaWorker();

    if (!_updateInProgress) {
        worker->addJob(job);
    }
    else {
        _waitingJobs.push_back(job);
    }
}

void SimulationAccessGpuImpl::jobsFinished()
{
	auto worker = _context->getCudaController()->getCudaWorker();
	auto finishedJobs = worker->getFinishedJobs(getObjectId());
	for (auto const& job : finishedJobs) {

		if (auto const& getDataForUpdateJob = boost::dynamic_pointer_cast<_GetDataForUpdateJob>(job)) {
			auto dataToUpdateTO = getDataForUpdateJob->getDataTO();
			updateDataToGpu(dataToUpdateTO, getDataForUpdateJob->getRect(), getDataForUpdateJob->getUpdateDescription());
			Q_EMIT dataUpdated();
		}

		if (auto const& getImageJob = boost::dynamic_pointer_cast<_GetImageJob>(job)) {
			Q_EMIT imageReady();
		}

		if (auto const& getDataForEditJob = boost::dynamic_pointer_cast<_GetDataForEditJob>(job)) {
			auto dataTO = getDataForEditJob->getDataTO();
			createDataFromGpuModel(dataTO, getDataForEditJob->getRect());
			_dataTOCache->releaseDataTO(dataTO);
			Q_EMIT dataReadyToRetrieve();
		}

		if (auto const& setDataJob = boost::dynamic_pointer_cast<_SetDataJob>(job)) {
			_dataTOCache->releaseDataTO(setDataJob->getDataTO());
			_updateInProgress = false;
			for (auto const& job : _waitingJobs) {
				worker->addJob(job);
			}
			_waitingJobs.clear();
		}
	}
}

void SimulationAccessGpuImpl::updateDataToGpu(DataAccessTO dataToUpdateTO, IntRect const& rect, DataChangeDescription const& updateDesc)
{
	DataConverter converter(dataToUpdateTO, _numberGen, _context->getSimulationParameters());
	converter.updateData(updateDesc);

	auto cudaWorker = _context->getCudaController()->getCudaWorker();
	CudaJob job = boost::make_shared<_SetDataJob>(getObjectId(), true, rect, dataToUpdateTO);
	cudaWorker->addJob(job);
}

void SimulationAccessGpuImpl::createDataFromGpuModel(DataAccessTO dataTO, IntRect const& rect)
{
	_lastDataRect = rect;

	DataConverter converter(dataTO, _numberGen, _context->getSimulationParameters());
	_dataCollected = converter.getDataDescription();
}

void SimulationAccessGpuImpl::metricCorrection(DataChangeDescription & data) const
{
	SpaceProperties* space = _context->getSpaceProperties();
	for (auto& cluster : data.clusters) {
		QVector2D origPos = cluster->pos.getValue();
		auto pos = origPos;
		space->correctPosition(pos);
		auto correctionDelta = pos - origPos;
		if (!correctionDelta.isNull()) {
			cluster->pos.setValue(pos);
		}
		for (auto& cell : cluster->cells) {
			cell->pos.setValue(cell->pos.getValue() + correctionDelta);
		}
	}
	for (auto& particle : data.particles) {
		QVector2D origPos = particle->pos.getValue();
		auto pos = origPos;
		space->correctPosition(pos);
		if (pos != origPos) {
			particle->pos.setValue(pos);
		}
	}
}

string SimulationAccessGpuImpl::getObjectId() const
{
	auto id = reinterpret_cast<long long>(this);
	std::stringstream stream;
	stream << SimulationAccessGpuId << id;
	return stream.str();
}

SimulationAccessGpuImpl::_DataTOCache::_DataTOCache(CudaConstants const& cudaConstants)
    : _cudaConstants(cudaConstants)
{
}

SimulationAccessGpuImpl::_DataTOCache::~_DataTOCache()
{
	for (DataAccessTO const& dataTO : _freeDataTOs) {
        deleteDataTO(dataTO);
	}
    for (DataAccessTO const& dataTO : _usedDataTOs) {
        deleteDataTO(dataTO);
    }
}

DataAccessTO SimulationAccessGpuImpl::_DataTOCache::getDataTO()
{
	DataAccessTO result;
	if (!_freeDataTOs.empty()) {
		result = *_freeDataTOs.begin();
		_freeDataTOs.erase(_freeDataTOs.begin());
		_usedDataTOs.emplace_back(result);
		return result;
	}
    result = getNewDataTO();
    _usedDataTOs.emplace_back(result);
	return result;
}

void SimulationAccessGpuImpl::_DataTOCache::releaseDataTO(DataAccessTO const & dataTO)
{
    auto usedDataTO = std::find_if(_usedDataTOs.begin(), _usedDataTOs.end(), [&dataTO](DataAccessTO const& usedDataTO) {
		return usedDataTO == dataTO;
	});
	if (usedDataTO != _usedDataTOs.end()) {
        _freeDataTOs.emplace_back(*usedDataTO);
		_usedDataTOs.erase(usedDataTO);
	}
}

DataAccessTO SimulationAccessGpuImpl::_DataTOCache::getNewDataTO()
{
    DataAccessTO result;
    result.numClusters = new int;
    result.numCells = new int;
    result.numParticles = new int;
    result.numTokens = new int;
    result.numStringBytes = new int;
    result.clusters = new ClusterAccessTO[_cudaConstants.MAX_CLUSTERS];
    result.cells = new CellAccessTO[_cudaConstants.MAX_CELLS];
    result.particles = new ParticleAccessTO[_cudaConstants.MAX_PARTICLES];
    result.tokens = new TokenAccessTO[_cudaConstants.MAX_TOKENS];
    result.stringBytes = new char[_cudaConstants.METADATA_DYNAMIC_MEMORY_SIZE];
    return result;
}

void SimulationAccessGpuImpl::_DataTOCache::deleteDataTO(DataAccessTO const & dataTO)
{
    delete dataTO.numClusters;
    delete dataTO.numCells;
    delete dataTO.numParticles;
    delete dataTO.numTokens;
    delete dataTO.numStringBytes;
    delete[] dataTO.clusters;
    delete[] dataTO.cells;
    delete[] dataTO.particles;
    delete[] dataTO.tokens;
    delete[] dataTO.stringBytes;
}
