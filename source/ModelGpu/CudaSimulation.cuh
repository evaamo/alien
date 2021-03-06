#pragma once

#include "ModelBasic/MonitorData.h"
#include "ModelBasic/ExecutionParameters.h"

#include "Definitions.cuh"

class CudaSimulation
{
public:
    CudaSimulation(
        int2 const& size,
        int timestep,
        SimulationParameters const& parameters,
        CudaConstants const& cudaConstants);
    ~CudaSimulation();

    void calcCudaTimestep();

    void getSimulationImage(int2 const& rectUpperLeft, int2 const& rectLowerRight, unsigned char* imageData);
    void getSimulationData(int2 const& rectUpperLeft, int2 const& rectLowerRight, DataAccessTO const& dataTO);
    void setSimulationData(int2 const& rectUpperLeft, int2 const& rectLowerRight, DataAccessTO const& dataTO);

    struct ApplyForceData
    {
        float2 startPos;
        float2 endPos;
        float2 force;
        bool onlyRotation;
    };
    void applyForce(ApplyForceData const& applyData);

    MonitorData getMonitorData();
    int getTimestep() const;
    void setTimestep(int timestep);

    void setSimulationParameters(SimulationParameters const& parameters);
    void setExecutionParameters(ExecutionParameters const& parameters);

    void clear();

private:
    void setCudaConstants(CudaConstants const& cudaConstants);
    void DEBUG_printNumEntries();

private:
    SimulationData* _cudaSimulationData;
    DataAccessTO* _cudaAccessTO;
    CudaMonitorData* _cudaMonitorData;
};
