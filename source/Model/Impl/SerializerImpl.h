#pragma once

#include <QObject>

#include "Model/Api/Serializer.h"
#include "Definitions.h"

class SerializerImpl
	: public Serializer
{
	Q_OBJECT

public:
	SerializerImpl(QObject *parent = nullptr);
	virtual ~SerializerImpl() = default;

	virtual void serialize(SimulationController* simController, SimulationAccess* access) override;
	virtual string const& retrieveSerializedSimulationContent() override;
	virtual string const& retrieveSerializedSimulation() override;

	virtual void deserializeSimulationContent(SimulationAccess* access, string const& content) const override;
	virtual pair<SimulationController*, SimulationAccess*> deserializeSimulation(string const& content) const override;

private:
	Q_SLOT void dataReadyToRetrieve();

	SimulationController* _simControllerForSerialization = nullptr;
	SimulationAccess* _simAccessForSerialization = nullptr;

	string _serializedUniverse;
	string _serializedSimulation;
};
