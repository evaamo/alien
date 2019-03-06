#pragma once

#include "Base.cuh"
#include "CudaInterface.cuh"
#include "Map.cuh"

struct Particle
{
	uint64_t id;
	float energy;
	float2 pos;
	float2 vel;

	//auxiliary data
	int locked;	//0 = unlocked, 1 = locked
	bool alive;
};

struct Cluster;
struct Cell
{
	uint64_t id;
	Cluster* cluster;
	float2 relPos;
	float2 absPos;
	float2 vel;
	float energy;
	int maxConnections;
	int numConnections;
	Cell* connections[MAX_CELL_BONDS];
	Cell* nextTimestep;

	//auxiliary data
	int protectionCounter;
	bool alive;
	int tag;
};

struct Cluster
{
	uint64_t id;
	float2 pos;
	float2 vel;
	float angle;
	float angularVel;
	float angularMass;
	int numCells;
	Cell* cells;

	//auxiliary data
	bool decompositionRequired;
	int locked;	//0 = unlocked, 1 = locked
	Cluster* clusterToFuse;
};

struct SimulationData
{
	int2 size;

	Cell **cellMap;
	Particle **particleMap;

	ArrayController<Cluster> clustersAC1;
	ArrayController<Cluster> clustersAC2;
	ArrayController<Cell> cellsAC1;
	ArrayController<Cell> cellsAC2;
	ArrayController<Particle> particlesAC1;
	ArrayController<Particle> particlesAC2;

	CudaNumberGenerator numberGen;
};
