#pragma once

struct Cell;
struct Cluster;
struct Token;
struct Particle;
struct Entities;

struct SimulationData;
struct CellAccessTO;
struct ClusterAccessTO;
struct DataAccessTO;
struct SimulationParameters;
struct CudaConstants;
class CudaMonitorData;

#define FP_PRECISION 0.00001

#define CUDA_THROW_NOT_IMPLEMENTED() printf("not implemented"); \
    while(true) {};
