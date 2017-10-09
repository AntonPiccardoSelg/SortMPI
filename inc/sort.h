#ifndef SORT_SORT_MPI_H
#define SORT_SORT_MPI_H

#include <vector>
#include <boost/mpi.hpp>

namespace SortMPI {
    template<typename SORT>
    class Sort {
    public:
        Sort(const boost::mpi::communicator &world) : world(world) {}

        template<typename T>
        void sort(std::vector<T> &toSort) {
            if (toSort.size() == 0) {
                return;
            }
            static_cast<SORT *>(this)->doSort(toSort);
        }

    protected:
        const boost::mpi::communicator &world;
    };
}

#endif // SORT_SORT_MPI_H
