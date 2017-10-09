#ifndef SORT_SORT_MPI_H
#define SORT_SORT_MPI_H

#include <vector>

template <typename SORT>
struct Sort {
    template<typename T>
    void sort(std::vector<T>& toSort) {
        if (toSort.size() == 0) {
            return;
        }
        static_cast<SORT*>(this)->doSort(toSort);
    }
};

#endif // SORT_SORT_MPI_H
