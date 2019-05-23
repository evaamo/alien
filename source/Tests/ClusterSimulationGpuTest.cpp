#include "SimulationGpuTestFramework.h"

class ClusterSimulationGpuTest
	: public SimulationGpuTestFramework
{
public:
	virtual ~ClusterSimulationGpuTest() = default;
};

/**
* Situation: horizontal collision of two cells where both move such that no pixel overlapping occurs
* Expected result: direction of movement of both cells changed
*/
TEST_F(ClusterSimulationGpuTest, DISABLED_testCollisionOfSingleCells_horizontal_noPixelOverlapping)
{
	DataDescription origData;
	auto cellEnergy = _parameters.cellFunctionConstructorOffspringCellEnergy;

	auto cellId1 = _numberGen->getId();
	auto cell1 = CellDescription().setId(cellId1).setPos({ 100, 100 }).setMaxConnections(0).setEnergy(cellEnergy);
	auto cluster1 = ClusterDescription().setId(_numberGen->getId()).setVel({ 0.1f, 0 }).setAngle(0).setAngularVel(0)
		.addCell(cell1);
	cluster1.setPos(cluster1.getClusterPosFromCells());
	origData.addCluster(cluster1);

	auto cellId2 = _numberGen->getId();
	auto cell2 = CellDescription().setId(cellId2).setPos({ 110, 100 }).setMaxConnections(0).setEnergy(cellEnergy);
	auto cluster2 = ClusterDescription().setId(_numberGen->getId()).setVel({ -0.1f, 0 }).setAngle(0).setAngularVel(0)
		.addCell(cell2);
	cluster2.setPos(cluster2.getClusterPosFromCells());
	origData.addCluster(cluster2);

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(150, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);

	ASSERT_EQ(2, newData.clusters->size());
	auto cellById = IntegrationTestHelper::getCellByCellId(newData);
	auto newCell1 = cellById.at(cellId1);
	auto newCell2 = cellById.at(cellId2);
	auto clusterById = IntegrationTestHelper::getClusterByCellId(newData);
	auto newCluster1 = clusterById.at(cellId1);
	auto newCluster2 = clusterById.at(cellId2);

	EXPECT_GE(99, newCell1.pos->x());
	EXPECT_TRUE(isCompatible(100.0f, newCell1.pos->y()));
	EXPECT_TRUE(isCompatible(QVector2D(-0.1f, 0), *newCluster1.vel));

	EXPECT_LE(111, newCell2.pos->x());
	EXPECT_TRUE(isCompatible(100.0f, newCell2.pos->y()));
	EXPECT_TRUE(isCompatible(QVector2D(0.1f, 0), *newCluster2.vel));
}


/**
* Situation: horizontal collision of two cells
* Expected result: direction of movement of both cells changed
*/
TEST_F(ClusterSimulationGpuTest, testCollisionOfSingleCells_horizontal)
{
	DataDescription origData;
	auto cellEnergy = _parameters.cellFunctionConstructorOffspringCellEnergy;

	auto cellId1 = _numberGen->getId();
	auto cell1 = CellDescription().setId(cellId1).setPos({ 100, 100 }).setMaxConnections(0).setEnergy(cellEnergy);
	auto cluster1 = ClusterDescription().setId(_numberGen->getId()).setVel({ 0.1f, 0 }).setAngle(0).setAngularVel(0)
		.addCell(cell1);
	cluster1.setPos(cluster1.getClusterPosFromCells());
	origData.addCluster(cluster1);

	auto cellId2 = _numberGen->getId();
	auto cell2 = CellDescription().setId(cellId2).setPos({ 110.2f, 100 }).setMaxConnections(0).setEnergy(cellEnergy);
	auto cluster2 = ClusterDescription().setId(_numberGen->getId()).setVel({ -0.1f, 0 }).setAngle(0).setAngularVel(0)
		.addCell(cell2);
	cluster2.setPos(cluster2.getClusterPosFromCells());
	origData.addCluster(cluster2);

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(150, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);

	ASSERT_EQ(2, newData.clusters->size());
	auto cellById = IntegrationTestHelper::getCellByCellId(newData);
	auto newCell1 = cellById.at(cellId1);
	auto newCell2 = cellById.at(cellId2);
	auto clusterById = IntegrationTestHelper::getClusterByCellId(newData);
	auto newCluster1 = clusterById.at(cellId1);
	auto newCluster2 = clusterById.at(cellId2);

	EXPECT_GE(99, newCell1.pos->x());
	EXPECT_TRUE(isCompatible(100.0f, newCell1.pos->y()));
	EXPECT_TRUE(isCompatible(QVector2D(-0.1f, 0), *newCluster1.vel));

	EXPECT_LE(111, newCell2.pos->x());
	EXPECT_TRUE(isCompatible(100.0f, newCell2.pos->y()));
	EXPECT_TRUE(isCompatible(QVector2D(0.1f, 0), *newCluster2.vel));
}

/**
* Situation: vertical collision of two cells
* Expected result: direction of movement of both cells changed
*/
TEST_F(ClusterSimulationGpuTest, testCollisionOfSingleCells_vertical)
{
	DataDescription origData;
	auto cellEnergy = _parameters.cellFunctionConstructorOffspringCellEnergy;

	auto cellId1 = _numberGen->getId();
	auto cell1 = CellDescription().setId(cellId1).setPos({ 100, 100 }).setMaxConnections(0).setEnergy(cellEnergy);
	auto cluster1 = ClusterDescription().setId(_numberGen->getId()).setVel({ 0, 0.1f }).setAngle(0).setAngularVel(0)
		.addCell(cell1);
	cluster1.setPos(cluster1.getClusterPosFromCells());
	origData.addCluster(cluster1);

	auto cellId2 = _numberGen->getId();
	auto cell2 = CellDescription().setId(cellId2).setPos({ 100, 110.2f }).setMaxConnections(0).setEnergy(cellEnergy);
	auto cluster2 = ClusterDescription().setId(_numberGen->getId()).setVel({ 0, -0.1f }).setAngle(0).setAngularVel(0)
		.addCell(cell2);
	cluster2.setPos(cluster2.getClusterPosFromCells());
	origData.addCluster(cluster2);

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(150, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);

	ASSERT_EQ(2, newData.clusters->size());
	auto cellById = IntegrationTestHelper::getCellByCellId(newData);
	auto newCell1 = cellById.at(cellId1);
	auto newCell2 = cellById.at(cellId2);
	auto clusterById = IntegrationTestHelper::getClusterByCellId(newData);
	auto newCluster1 = clusterById.at(cellId1);
	auto newCluster2 = clusterById.at(cellId2);

	EXPECT_GE(99, newCell1.pos->y());
	EXPECT_TRUE(isCompatible(100.0f, newCell1.pos->x()));
	EXPECT_TRUE(isCompatible(QVector2D(0, -0.1f), *newCluster1.vel));

	EXPECT_LE(111, newCell2.pos->y());
	EXPECT_TRUE(isCompatible(100.0f, newCell2.pos->x()));
	EXPECT_TRUE(isCompatible(QVector2D(0, 0.1f), *newCluster2.vel));
}

/**
* Situation:
*	- center collision of two horizontal cell clusters
*	- first cluster has no velocity while second cluster moves upward
* Expected result: first cluster moves upward while second cluster stand stills
*/
TEST_F(ClusterSimulationGpuTest, testCenterCollisionOfParallelLineClusters)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0));
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 100, 110 }, QVector2D{ 0, -0.1f }, 0));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(150, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_EQ(100, cluster.pos->x());
		EXPECT_GE(100, cluster.pos->y());
		EXPECT_TRUE(isCompatible(0.0f, cluster.vel->x()));
		EXPECT_TRUE(isCompatible(-0.1f, cluster.vel->y()));
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_EQ(100, cluster.pos->x());
		EXPECT_LE(100, cluster.pos->y());
		EXPECT_TRUE(isCompatible(QVector2D(0, 0), *cluster.vel));
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: horizontal collision of two line clusters at boundary
* Expected result: direction of movement of both cells changed accordingly
*/
TEST_F(ClusterSimulationGpuTest, testHorizontalCenterCollisionOfParallelLineClusters_atUniverseBoundary)
{
	DataDescription origData;
	auto size = _spaceProp->getSize();
	origData.addCluster(createHorizontalCluster(10, QVector2D{ size.x - 5.0f, 100 }, QVector2D{ 0.1f, 0 }, 0));
	origData.addCluster(createHorizontalCluster(10, QVector2D{ size.x + 5.0f, 100 }, QVector2D{ -0.1f,0 }, 0));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(20, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);

	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_TRUE(isCompatible(-0.1f, cluster.vel->x()));
		EXPECT_TRUE(isCompatible(0.0f, cluster.vel->y()));
		EXPECT_TRUE(isCompatible(0.0, *cluster.angularVel));
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_TRUE(isCompatible(0.1f, cluster.vel->x()));
		EXPECT_TRUE(isCompatible(0.0f, cluster.vel->y()));
		EXPECT_TRUE(isCompatible(0.0, *cluster.angularVel));
	}
	checkDistancesToConnectingCells(newData);
}


/**
* Situation: vertical collision of two line clusters at boundary
* Expected result: direction of movement of both cells changed accordingly
*
* Hint if failed: collision calculated twice?
*/
TEST_F(ClusterSimulationGpuTest, testVerticalCenterCollisionOfParallelLineClusters_atUniverseBoundary)
{
	auto size = _spaceProp->getSize();

	DataDescription origData;
	origData.addCluster(createHorizontalCluster(10, QVector2D{ 100, size.y + 0.5f }, QVector2D{ 0, -0.1f }, 0.0));
	origData.addCluster(createHorizontalCluster(10, QVector2D{ 100, size.y - 0.5f }, QVector2D{ 0, 0.1f }, 0.0));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_TRUE(isCompatible(QVector2D(0.0f, 0.1f), *cluster.vel));
		EXPECT_TRUE(isCompatible(0.0, *cluster.angularVel));
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_TRUE(isCompatible(QVector2D(0.0f, -0.1f), *cluster.vel));
		EXPECT_TRUE(isCompatible(0.0, *cluster.angularVel));
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}


/**
* Situation:
*	- sidewise collision of two horizontal cell clusters
*	- first cluster has no velocity while second cluster moves upward
* Expected result: both clusters move upwards and rotate counterclockwise
*/
TEST_F(ClusterSimulationGpuTest, testSidewiseCollisionOfParallelLineClusters)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0));
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 199, 110 }, QVector2D{ 0, -0.1f }, 0));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(120, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_GE(NearlyZero, cluster.vel->x());
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_LE(-NearlyZero, cluster.vel->x());
		EXPECT_GE(NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation:
*	- sidewise collision of two rectangular cell clusters
*	- first cluster has no velocity while second cluster moves upward
* Expected result: both clusters move upwards and rotate counterclockwise
*/
TEST_F(ClusterSimulationGpuTest, testSidewiseCollisionOfRectangleClusters)
{
	DataDescription origData;
	origData.addCluster(createRectangularCluster({ 10, 10 }, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }));
	origData.addCluster(createRectangularCluster({ 10, 10 }, QVector2D{ 105, 120 }, QVector2D{ 0, -0.5f }));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(23, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation:
*	- sidewise collision of two rectangular cell clusters
*	- both clusters have velocity and angular velocity
* Expected result: energy is conserved
*/
TEST_F(ClusterSimulationGpuTest, testSidewiseCollisionOfRectangleClusters_withAngularVelocities)
{
	DataDescription origData;
	auto cluster1 = createRectangularCluster({ 20, 20 }, QVector2D{ 457.46f, 356.37f }, QVector2D{ -0.011f, -0.077f });
	cluster1.angle = 1.409;
	cluster1.angularVel = -0.271;
	auto cluster2 = createRectangularCluster({ 20, 20 }, QVector2D{ 476.24f, 341.08f }, QVector2D{ -0.088f, 0.061f });
	cluster2.angle = 307.34;
	cluster2.angularVel = 0.394;
	origData.addCluster(cluster1);
	origData.addCluster(cluster2);
	uint64_t clusterId1 = cluster1.id;
	uint64_t clusterId2 = cluster2.id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation:
*	- sidewise collision of two orthogonal cell clusters
*	- first cluster has no velocity while second cluster moves upward
* Expected result:
*	- first cluster moves upward and rotate counterclockwise
*	- second cluster does not move on x axis and does not rotate
*/
TEST_F(ClusterSimulationGpuTest, testSidewiseCollisionOfOrthogonalLineClusters)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0));
	origData.addCluster(createVerticalCluster(100, QVector2D{ 148, 160 }, QVector2D{ 0, -0.1f }));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(120, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_GE(NearlyZero, cluster.vel->x());
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_TRUE(isCompatible(0.0f, cluster.vel->x()));
		EXPECT_GE(NearlyZero, cluster.vel->y());
		EXPECT_TRUE(abs(*cluster.angularVel) < 0.01);
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation:
*	- sidewise collision of two traversal cell clusters
*	- first cluster is arranged horizontal and has no velocity
*	- second cluster is below the first one, sloped at 45 degree and moves upward
* Expected result:
*	- first cluster moves upward and rotate clockwise
*	- second cluster moves upward and rotate counterclockwise
*/
TEST_F(ClusterSimulationGpuTest, testSidewiseCollisionOfTraversalLineClusters)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0));
	origData.addCluster(createLineCluster(100, QVector2D{ 100, 145 }, QVector2D{ 0, -0.5f }, 45, 0));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(40, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_TRUE(abs(cluster.vel->x()) < NearlyZero);
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_LE(NearlyZero, *cluster.angularVel);
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_TRUE(abs(cluster.vel->x()) < NearlyZero);
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation:
*	- sidewise collision of two traversal cell clusters
*	- first cluster is arranged horizontal and has no velocity
*	- second cluster is below the first one, sloped at 45 degree and moves upward
*	- simulate until second collision occur
* Expected result:
*	- velocity of first cluster: x: positive, y: negative, angular vel: negative
*	- velocity of second cluster: x: negative, y: negative, angular vel: positive
*/
TEST_F(ClusterSimulationGpuTest, testSidewiseCollisionOfTraversalLineClusters_waitUntilSecondCollision)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0));
	origData.addCluster(createLineCluster(100, QVector2D{ 100, 145 }, QVector2D{ 0, -0.25f }, 45, 0));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(200, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_LE(NearlyZero, cluster.vel->x());
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_GE(-NearlyZero, cluster.vel->x());
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_LE(NearlyZero, *cluster.angularVel);
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: same as testSidewiseCollisionOfTraversalLineClusters_waitUntilSecondCollision but with high velocities
*/
TEST_F(ClusterSimulationGpuTest, DISABLED_testSidewiseCollisionOfTraversalLineClusters_waitUntilSecondCollision_faster)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(100, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0));
	origData.addCluster(createLineCluster(100, QVector2D{ 100, 145 }, QVector2D{ 0, -0.25f }, 45, 0));
	uint64_t clusterId1 = origData.clusters->at(0).id;
	uint64_t clusterId2 = origData.clusters->at(1).id;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(200, _controller);

	IntRect rect = { { 0, 0 },{ _universeSize.x, _universeSize.y } };
	DataDescription newData = IntegrationTestHelper::getContent(_access, rect);
	ASSERT_EQ(2, newData.clusters->size());

	auto clusterById = IntegrationTestHelper::getClusterByClusterId(newData);
	{
		auto cluster = clusterById.at(clusterId1);
		EXPECT_LE(NearlyZero, cluster.vel->x());
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_GE(-NearlyZero, *cluster.angularVel);
	}

	{
		auto cluster = clusterById.at(clusterId2);
		EXPECT_GE(-NearlyZero, cluster.vel->x());
		EXPECT_GE(-NearlyZero, cluster.vel->y());
		EXPECT_LE(NearlyZero, *cluster.angularVel);
	}

	checkKineticEnergy(origData, newData);
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: cluster with cross structure where middle cell connecting 4 parts has low energy
* Expected result: cluster decomposes into 4 parts
*/
TEST_F(ClusterSimulationGpuTest, testDecomposeClusterAfterLowEnergy)
{
	DataDescription origData;
	{
		auto cluster = ClusterDescription().setId(_numberGen->getId()).setVel({ 0, 0 }).setAngle(0).setAngularVel(0);
		for (int i = 0; i < 30; ++i) {
			auto cell = CellDescription().setId(_numberGen->getId()).setPos({ 100, 100 + float(i) }).setMaxConnections(4);
			if (15 == i) {
				cell.setEnergy(_parameters.cellMinEnergy / 2);
			}
			else {
				cell.setEnergy(_parameters.cellMinEnergy * 2);
			}
			cluster.addCell(cell);
		}
		auto leftCell = CellDescription().setId(_numberGen->getId()).setPos({ 99, 115 }).setMaxConnections(4).setEnergy(_parameters.cellMinEnergy * 2);
		cluster.addCell(leftCell);
		auto rightCell = CellDescription().setId(_numberGen->getId()).setPos({ 101, 115 }).setMaxConnections(4).setEnergy(_parameters.cellMinEnergy * 2);
		cluster.addCell(rightCell);

		for (int i = 0; i < 30; ++i) {
			list<uint64_t> connectingCells;
			if (i > 0) {
				connectingCells.emplace_back(cluster.cells->at(i - 1).id);
			}
			if (i < 30 - 1) {
				connectingCells.emplace_back(cluster.cells->at(i + 1).id);
			}
			cluster.cells->at(i).setConnectingCells(connectingCells);
		}
		cluster.cells->at(30).addConnection(cluster.cells->at(15).id);
		cluster.cells->at(15).addConnection(cluster.cells->at(30).id);
		cluster.cells->at(31).addConnection(cluster.cells->at(15).id);
		cluster.cells->at(15).addConnection(cluster.cells->at(31).id);

		cluster.setPos(cluster.getClusterPosFromCells());
		origData.addCluster(cluster);
	}

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(3, _controller);

	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	auto numClusters = newData.clusters->size();
	ASSERT_EQ(4, numClusters);

	unordered_map<int, vector<ClusterDescription>> clustersBySize;
	for (ClusterDescription const& cluster : *newData.clusters) {
		int numCells = cluster.cells ? cluster.cells->size() : 0;
		clustersBySize[numCells].emplace_back(cluster);
	};
	ASSERT_LE(2, clustersBySize.at(1).size());
	ASSERT_EQ(1, clustersBySize.at(14).size());
	ASSERT_EQ(1, clustersBySize.at(15).size());

	unordered_map<uint64_t, CellDescription> origCellById = IntegrationTestHelper::getCellByCellId(origData);
	for (ClusterDescription const& cluster : *newData.clusters) {
		EXPECT_EQ(cluster.getClusterPosFromCells(), *cluster.pos);
		for (CellDescription const& cell : *cluster.cells) {
			CellDescription const& origCell = origCellById.at(cell.id);
			EXPECT_TRUE(isCompatible(cell.pos, origCell.pos));
		}
	}
	checkDistancesToConnectingCells(newData);
}

/**
* Situation:
*	- cluster with line structure where middle cell has low energy
*	- cluster rotates
* Expected result: cluster decomposes into 2 parts
*/
TEST_F(ClusterSimulationGpuTest, testDecomposeClusterAfterLowEnergy_duringRotation)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(5, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 1.0));
	origData.addCluster(createHorizontalCluster(5, QVector2D{ 200, 100 }, QVector2D{ 0, 0 }, 1.0));	//second cluster for comparison

	auto lowEnergy = _parameters.cellMinEnergy / 2.0;
	origData.clusters->at(0).cells->at(2).energy = lowEnergy;
	origData.clusters->at(0).angle = 90;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	auto newClusterById = IntegrationTestHelper::getClusterByCellId(newData);
	ClusterDescription refCluster = newClusterById.at(origData.clusters->at(1).cells->at(0).id);
	{
		ClusterDescription firstFragment = newClusterById.at(origData.clusters->at(0).cells->at(0).id);

		set<uint64_t> firstFragmentCellIds;
		for (int i = 0; i < 2; ++i) {
			firstFragmentCellIds.insert(origData.clusters->at(1).cells->at(i).id);
		}
		Physics::Velocities velocities = calcVelocitiesOfClusterPart(refCluster, firstFragmentCellIds);
		EXPECT_TRUE(isCompatible(velocities.linear, *firstFragment.vel));
		EXPECT_TRUE(isCompatible(velocities.angular, *firstFragment.angularVel));
	}
	{
		ClusterDescription secondFragment = newClusterById.at(origData.clusters->at(0).cells->at(4).id);

		set<uint64_t> secondFragmentCellIds;
		for (int i = 3; i < 5; ++i) {
			secondFragmentCellIds.insert(origData.clusters->at(1).cells->at(i).id);
		}
		Physics::Velocities velocities = calcVelocitiesOfClusterPart(refCluster, secondFragmentCellIds);
		EXPECT_TRUE(isCompatible(velocities.linear, *secondFragment.vel));
		EXPECT_TRUE(isCompatible(velocities.angular, *secondFragment.angularVel));
	}
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: two clusters are situated very close
* Expected result: the cells of the smaller clusters are destroyed
*/
TEST_F(ClusterSimulationGpuTest, testDestructionOfTooCloseCells)
{
    _parameters.radiationProb = 0;    //exclude radiation
    _context->setSimulationParameters(_parameters);

    float closeDistance = static_cast<float>(_parameters.cellMinDistance) / 2.0f;

	DataDescription origData;
	origData.addCluster(createHorizontalCluster(5, QVector2D{ 100.5f, 100.5f }, QVector2D{ 0, 0 }, 0.0));
	origData.addCluster(createHorizontalCluster(3, QVector2D{ 100.5f, 100.5f + closeDistance }, QVector2D{ 0, 0 }, 0.0));

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	ASSERT_EQ(1, newData.clusters->size());
	EXPECT_EQ(5, newData.clusters->at(0).cells->size());
	checkDistancesToConnectingCells(newData);
    checkEnergy(origData, newData);
}

/**
* Situation: two horizontal clusters are approaching each others vertically above critical speed
* Expected result: fusion should take place
*/
TEST_F(ClusterSimulationGpuTest, testFusionOfHorizontalClusters)
{
	float velocity = static_cast<float>(_parameters.cellFusionVelocity) + 0.1f;

	DataDescription origData;
	origData.addCluster(createHorizontalCluster(10, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0.0));
	origData.addCluster(createHorizontalCluster(12, QVector2D{ 100, 105 }, QVector2D{ 0, -velocity }, 0.0));
	setMaxConnections(origData.clusters->at(0), 3);
	setMaxConnections(origData.clusters->at(1), 3);

	IntegrationTestHelper::updateData(_access, origData);
	int duration = static_cast<int>((5.0f / velocity) + 5);
	IntegrationTestHelper::runSimulation(duration, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	ASSERT_EQ(1, newData.clusters->size());
	EXPECT_EQ(22, newData.clusters->at(0).cells->size());
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: two horizontal clusters are approaching each others vertically below critical speed
* Expected result: fusion should take place
*/
TEST_F(ClusterSimulationGpuTest, testNoFusionOfHorizontalClusters)
{
	float velocity = static_cast<float>(_parameters.cellFusionVelocity) - 0.1f;

	DataDescription origData;
	origData.addCluster(createHorizontalCluster(10, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }, 0.0));
	origData.addCluster(createHorizontalCluster(12, QVector2D{ 100, 105 }, QVector2D{ 0, -velocity }, 0.0));
	setMaxConnections(origData.clusters->at(0), 3);
	setMaxConnections(origData.clusters->at(1), 3);

	IntegrationTestHelper::updateData(_access, origData);
	int duration = static_cast<int>((5.0f / velocity) + 5);
	IntegrationTestHelper::runSimulation(duration, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	ASSERT_EQ(2, newData.clusters->size());
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: two line clusters are approaching each others during rotation
* Expected result: fusion should take place with correct (angular) velocity
*/
TEST_F(ClusterSimulationGpuTest, testFusionOfLineClusters_duringRotation)
{
	float fusionVelocity = static_cast<float>(_parameters.cellFusionVelocity) + 0.1f;

	DataDescription origData;
	origData.addCluster(createHorizontalCluster(10, QVector2D{ 100, 100.1f }, QVector2D{ 0, 0 }, 1.0));
	origData.addCluster(createHorizontalCluster(12, QVector2D{ 100, 101 }, QVector2D{ 0, -fusionVelocity }, 1.0));
	setMaxConnections(origData.clusters->at(0), 3);
	setMaxConnections(origData.clusters->at(1), 3);
	origData.clusters->at(0).angle = 90;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	ASSERT_EQ(1, newData.clusters->size());

	auto refCluster1 = createLineCluster(10, QVector2D{ 100, 100.1f }, QVector2D{ 0, 0 }, 1.0, 1.0);
	auto refCluster2 = createLineCluster(12, QVector2D{ 100, 101 - fusionVelocity }, QVector2D{ 0, -fusionVelocity }, 1.0, 1.0);
	auto expectedFusionVelocity = calcVelocitiesOfFusion(refCluster1, refCluster2);

	auto fusedCluster = newData.clusters->at(0);
	EXPECT_EQ(22, fusedCluster.cells->size());
	EXPECT_TRUE(isCompatible(*fusedCluster.angularVel, expectedFusionVelocity.angular));
	EXPECT_TRUE(isCompatible(*fusedCluster.vel, expectedFusionVelocity.linear));
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: two horizontal clusters with a position offset are approaching
* Expected result: fusion should take place with correct (angular) velocity
*/
TEST_F(ClusterSimulationGpuTest, testFusionOfHorizontalClusters_partialContact)
{
	float fusionVelocity = static_cast<float>(_parameters.cellFusionVelocity) + 0.1f;

	DataDescription origData;
	origData.addCluster(createHorizontalCluster(10, QVector2D{ 100, 100.1f }, QVector2D{ 0, 0 }, 0.0));
	origData.addCluster(createHorizontalCluster(12, QVector2D{ 103, 101 }, QVector2D{ 0, -fusionVelocity }, 0.0));
	setMaxConnections(origData.clusters->at(0), 3);
	setMaxConnections(origData.clusters->at(1), 3);
	origData.clusters->at(0).angle = 90;

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	ASSERT_EQ(1, newData.clusters->size());

	auto refCluster1 = createLineCluster(10, QVector2D{ 100, 100.1f }, QVector2D{ 0, 0 }, 0.0, 0.0);
	auto refCluster2 = createLineCluster(12, QVector2D{ 103, 101 - fusionVelocity }, QVector2D{ 0, -fusionVelocity }, 0.0, 0.0);
	auto expectedFusionVelocity = calcVelocitiesOfFusion(refCluster1, refCluster2);

	auto fusedCluster = newData.clusters->at(0);
	EXPECT_EQ(22, fusedCluster.cells->size());
	EXPECT_TRUE(isCompatible(*fusedCluster.angularVel, expectedFusionVelocity.angular));
	EXPECT_TRUE(isCompatible(*fusedCluster.vel, expectedFusionVelocity.linear));
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: two horizontal clusters are approaching each others horizontally at universe boundary
* Expected result: fusion should take place
*/
TEST_F(ClusterSimulationGpuTest, testFusionOfHorizontalClusters_atUniverseBoundary)
{
	auto fusionVelocity = static_cast<float>(_parameters.cellFusionVelocity) + 0.1f;

	DataDescription origData;
	origData.addCluster(createHorizontalCluster(10, QVector2D{ _universeSize.x - 5.0f, 100 }, QVector2D{ 0, 0 }, 0.0));
	origData.addCluster(createHorizontalCluster(12, QVector2D{ _universeSize.x + 6.0f, 100 }, QVector2D{ -fusionVelocity, 0 }, 0.0));
	setMaxConnections(origData.clusters->at(0), 3);
	setMaxConnections(origData.clusters->at(1), 3);

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	ASSERT_EQ(1, newData.clusters->size());
	EXPECT_EQ(22, newData.clusters->at(0).cells->size());
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: one fast moving but not rotating cluster
* Expected result: no cells are destroyed
*/
TEST_F(ClusterSimulationGpuTest, testFastMovingCluster)
{
	float cellMaxForce = static_cast<float>(_parameters.cellMaxForce);
	DataDescription origData;
	origData.addCluster(createRectangularCluster({ 10, 10 }, QVector2D{ _universeSize.x / 2.0f, _universeSize.y / 2.0f }, QVector2D{ cellMaxForce*3.0f, 0.0f }));

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(1, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	ASSERT_EQ(1, newData.clusters->size());
	EXPECT_EQ(100, newData.clusters->at(0).cells->size());
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: cluster rotating very very fast
* Expected result: cells are destroyed because of high forces
*/
TEST_F(ClusterSimulationGpuTest, testFastRotatingCluster)
{
	DataDescription origData;
	origData.addCluster(createHorizontalCluster(51, QVector2D{ _universeSize.x / 2.0f, _universeSize.y / 2.0f }, QVector2D(), 20.0));

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(100, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });

	int numCells = 0;
	if (newData.clusters) {
		for (auto const& cluster : *newData.clusters) {
			numCells += cluster.cells->size();
		}
	}
	EXPECT_LE(2, newData.clusters->size());
	checkDistancesToConnectingCells(newData);
}

/**
* Situation:
*	- destruction of cells in two overlapping clusters
*	- important: NUM_THREADS_PER_BLOCK should be at least 128
* Fixed error: problem due to missing __synchtreads call
* Expected result: no crash
*/
TEST_F(ClusterSimulationGpuTest, regressionTest_overlappingRectangleClusters_manyThreadsPerBlocks)
{
	float closeDistance = static_cast<float>(_parameters.cellMinDistance) / 2.0f;

	DataDescription origData;
	origData.addCluster(createRectangularCluster({ 20, 20 }, QVector2D{ 100, 100 }, QVector2D{ 0, 0 }));
	origData.addCluster(createRectangularCluster({ 20, 20 }, QVector2D{ 105.0f + closeDistance, 105 }, QVector2D{ 0, 0 }));

	IntegrationTestHelper::updateData(_access, origData);
	EXPECT_NO_THROW(IntegrationTestHelper::runSimulation(1, _controller));
}

/**
* Situation: many overlapping and fast moving clusters
* Fixed errors:
*	- crash in case of three fusions (error in DoubleLocker)
*	- fusion may lead to wrong CellData::numConnections
* Expected result: no crash (test should be run at least 10 times)
*/
TEST_F(ClusterSimulationGpuTest, regressionTest_manyOverlappingRectangleClusters)
{
	float closeDistance = static_cast<float>(_parameters.cellMinDistance) / 2.0f;

	DataDescription origData;
	for (int i = 0; i < 40; ++i) {
		origData.addCluster(createRectangularCluster({ 20, 20 }, QVector2D{ 100.0f + 5.0f*i, 100 }, QVector2D{ -2.0f + 3.2f*i, -2.0f + 3.2f*i }));
	}

	IntegrationTestHelper::updateData(_access, origData);
	EXPECT_NO_THROW(IntegrationTestHelper::runSimulation(300, _controller));
}

/**
* Situation:
*	- many moving rectangular clusters
*	- important: NUM_THREADS_PER_BLOCK should be at least 128 and run in release build
* Fixed error: distance to connecting cells are too large (calculation of invRotMatrix in processingDataCopyWithDecomposition)
* Expected result: distance to connecting cells are admissible
*/
TEST_F(ClusterSimulationGpuTest, regressionTest_manyRectangleClusters_manyThreadsPerBlocks)
{
	DataDescription origData;
	for (int i = 0; i < 20; ++i) {
		origData.addCluster(createRectangularCluster({ 7, 40 }));
	}

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(300, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });
	checkDistancesToConnectingCells(newData);
}

/**
* Situation: many moving rectangular clusters concentrated at boundary
* Fixed error: distance to connecting cells are too large in release build
*			   due to missing initialization in BasicMap::correctionIncrement
* Expected result: distance to connecting cells are admissible
*/
TEST_F(ClusterSimulationGpuTest, regressionTest_manyRectangleClusters_concentratedAtUniverseBoundary)
{
	DataDescription origData;
	for (int i = 0; i < 100; ++i) {
		origData.addCluster(createRectangularCluster({ 7, 40 },
			QVector2D{
			static_cast<float>(_numberGen->getRandomReal(-20, -20 + 40)),
			static_cast<float>(_numberGen->getRandomReal(-20, -20 + 40)) }));
	}

	IntegrationTestHelper::updateData(_access, origData);
	IntegrationTestHelper::runSimulation(50, _controller);
	DataDescription newData = IntegrationTestHelper::getContent(_access, { { 0, 0 },{ _universeSize.x, _universeSize.y } });
	checkDistancesToConnectingCells(newData);
}