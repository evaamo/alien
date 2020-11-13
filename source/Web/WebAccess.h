#pragma once

#include "Definitions.h"

#include "SimulationInfo.h"

class WEB_EXPORT WebAccess : public QObject
{
    Q_OBJECT
public:
    virtual ~WebAccess() = default;

    virtual void requestSimulationInfos() = 0;
    virtual void requestConnectToSimulation(string const& simulationId, string const& password) = 0;
    virtual void requestTask(string const& simulationId) = 0;
    virtual void requestDisconnect(string const& simulationId, string const& token) = 0;

    Q_SIGNAL void simulationInfosReceived(vector<SimulationInfo> simulationInfos);
    Q_SIGNAL void connectToSimulationReceived(optional<string> token);
    Q_SIGNAL void error(string message);

};