#ifndef MERGE_SORT_MPI_MERGE_SORT_H
#define MERGE_SORT_MPI_MERGE_SORT_H

#include <algorithm>
#include <boost/mpi.hpp>
#include "sort.h"
#include <iostream>


namespace mpi = boost::mpi;


class MergeSort : public Sort<MergeSort> {
public:
    MergeSort(const mpi::communicator& world) : Sort(world) {}

    template<typename T>
    void doSort(std::vector<T>& toSort) const {
        // Scatter the input data from the master rank to the slave ranks
        std::vector<T> dataOnRank;
        scatterData(world, toSort, dataOnRank);

        // Perform the merge sort on the partial data sets
        dataOnRank = mergeSort(dataOnRank);

        // Gather the data from the slave ranks
        gatherData(world, dataOnRank, toSort);
    }

private:
    // ------------------------------------------------------------------------------
    // Actual merge algorithm
    // ------------------------------------------------------------------------------
    template<typename T>
    std::vector<T> mergeSort(std::vector<T>& toSort) const {
        if (toSort.size() == 1) {
            return toSort;
        }

        // Split the container
        std::size_t splitPoint = toSort.size()/2;
        auto leftStart = toSort.begin();
        auto leftEnd = leftStart + splitPoint;
        auto rightStart = leftEnd;
        auto rightEnd = toSort.end();

        std::vector<T> left(leftStart, leftEnd);
        std::vector<T> right(rightStart, rightEnd);

        // Recursive split
        auto leftSorted = mergeSort(left);
        auto rightSorted = mergeSort(right);

        // Merge
        return merge(leftSorted, rightSorted);
    }

    template<typename T>
    std::vector<T> merge(std::vector<T>& left, std::vector<T>& right) const {
        auto leftIt = left.begin();
        auto rightIt = right.begin();
        std::vector<T> merged;
        merged.reserve(left.size() + right.size());

        while (leftIt != left.end() && rightIt != right.end()) {
            if (*leftIt < *rightIt) {
                merged.push_back(*leftIt);
                ++leftIt;
            } else {
                merged.push_back(*rightIt);
                ++rightIt;
            }
        }

        while (leftIt != left.end()) {
            merged.push_back(*leftIt);
            ++leftIt;
        }

        while(rightIt != right.end()) {
            merged.push_back(*rightIt);
            ++rightIt;
        }
        return merged;
    }

    // ------------------------------------------------------------------------------
    // MPI handling
    // ------------------------------------------------------------------------------
    template<typename T>
    void scatterData(const mpi::communicator& world, const std::vector<T>& toSort,
                     std::vector<T>& dataForRank) const {
        const auto totalSize = getTotalSize(world, static_cast<int>(toSort.size()));
        dataForRank.resize(totalSize);

        std::vector<int> sizes(world.size());
        setSizes(world, sizes, static_cast<int>(toSort.size()));

        mpi::scatterv(world, toSort, sizes, dataForRank.data(), 0);
    }

    template<typename T>
    void gatherData(const mpi::communicator& world,
                    const std::vector<T>& dataOnRank,
                    std::vector<T>& toSort) const {
        // We gather all the sub arrays in rank 0
        if (world.rank() == 0) {
            // Gather the data
            std::vector<int> sizes(world.size());
            setSizes(world, sizes, static_cast<int>(toSort.size()));
            mpi::gatherv(world, dataOnRank, toSort.data(), sizes, 0);

            // Merge all sub arrays on the main array
            finalMerge(toSort, sizes);
        } else {
            mpi::gatherv(world, dataOnRank, 0);
        }
    }

    template<typename T>
    void finalMerge(std::vector<T>& toSort, const std::vector<int>& sizes) const {
        // Could make this part MPI style or at least use OpenMP.
        const auto middleAndStopIndices = getMiddleAndStopIndex(sizes);
        for (const auto& indices : middleAndStopIndices) {
            auto start = toSort.begin();
            auto middle = start + indices.first;
            auto end = start + indices.second;
            std::inplace_merge(start, middle, end);
        }
    }

    std::vector<std::pair<size_t, size_t>> getMiddleAndStopIndex(const std::vector<int>& sizes) const {
        std::vector<std::pair<size_t, size_t>> middleAndStopIndex;
        int startIndex = 0;
        for (auto index = 1u; index < sizes.size(); ++index) {
            const auto middleIndex = startIndex + sizes[index-1];
            const auto stopIndex = middleIndex + sizes[index];
            middleAndStopIndex.emplace_back(static_cast<size_t>(middleIndex), static_cast<size_t>(stopIndex));
            startIndex = middleIndex;
        }
        return middleAndStopIndex;
    };


    void setSizes(const mpi::communicator& world, std::vector<int>& sizes,
                  const size_t totalNumberOfElements) const {
        const auto numberOfRanks = world.size();

        // Get number of elements per rank
        const auto numberOfElementsToProcess = totalNumberOfElements / numberOfRanks;
        const auto elementsToAddToMasterRank = totalNumberOfElements % numberOfRanks;

        sizes[0] = numberOfElementsToProcess + elementsToAddToMasterRank;
        for (auto index = 1u; index < static_cast<size_t>(numberOfRanks); ++index) {
            sizes[index] = numberOfElementsToProcess;
        }
    };

    int getTotalSize(const mpi::communicator& world,
                     const int totalNumberOfElements) const {
        const auto numberOfRanks = world.size();

        // Get number of elements per rank
        const auto numberOfElementsToProcess = totalNumberOfElements / numberOfRanks;
        const auto elementsToAddToMasterRank = totalNumberOfElements % numberOfRanks;

        auto totalSize = numberOfElementsToProcess;
        if (world.rank() == 0) {
            totalSize += elementsToAddToMasterRank;
        }
        return totalSize;
    }
};

#endif //MERGE_SORT_MPI_MERGE_SORT_H
