#ifndef MERGE_SORT_MPI_MERGE_SORT_H
#define MERGE_SORT_MPI_MERGE_SORT_H

#include "sort.h"


class MergeSort : public Sort<MergeSort> {
public:
    template<typename T>
    void doSort(std::vector<T>& toSort) {
        toSort = mergeSort(toSort);
    }
private:
    template<typename T>
    std::vector<T> mergeSort(std::vector<T>& toSort) {
        // If we have one or
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
    std::vector<T> merge(std::vector<T>& left, std::vector<T>& right) {
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
};

#endif //MERGE_SORT_MPI_MERGE_SORT_H
