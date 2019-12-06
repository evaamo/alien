#pragma once

#include "Definitions.cuh"

#include "Base.cuh"
#include "Array.cuh"
#include "List.cuh"

#include "Cluster.cuh"

class ClustersByMapSection
{
public:
    __host__ __inline__ void
    init(int2 const& universeSize, int2 const& sectionSize)
    {
        _numSections = { universeSize.x / sectionSize.x, universeSize.y / sectionSize.y };
        _sectionSize = sectionSize;
        _clusterListBySectionIndex.init(_numSections.x *_numSections.y);
    }

    __host__ __inline__ void free()
    {
        _clusterListBySectionIndex.free();
    }

    __device__ __inline__ void reset_gridCall()
    {
        auto const partition = calcPartition(
            _numSections.x * _numSections.y, threadIdx.x + blockIdx.x * blockDim.x, blockDim.x * gridDim.x);
        for (int index = partition.startIndex; index <= partition.endIndex; ++index) {
            _clusterListBySectionIndex.at(index).init();
        }
    }

    __device__ __inline__ void insert(Cluster* cluster, DynamicMemory* dynamicMemory)
    {
        auto const intPos = toInt2(cluster->pos);
        auto const section = int2{ intPos.x / _sectionSize.x, intPos.y / _sectionSize.y };
        auto& clusterList = _clusterListBySectionIndex.at(section.x + section.y * _numSections.x);
        clusterList.pushBack(cluster, dynamicMemory);
    }

    __device__ __inline__ List<Cluster*> const& getClusters(int2 const& section)
    {
        return _clusterListBySectionIndex.at(section.x + section.y * _numSections.x);
    }

private:
    int2 _numSections;
    int2 _sectionSize;
    Array<List<Cluster*>> _clusterListBySectionIndex;
};
