#ifndef SORT_SORT_MPI_H
#define SORT_SORT_MPI_H

#include <vector>
#include <type_traits>
#include <boost/mpi.hpp>


namespace SortMPI {
    template<typename SORT>
    class Sort {
    public:
        Sort(const boost::mpi::communicator &world) : world(world) {}

        template<typename T>
        void sort(std::vector<T> &toSort) {
            static_assert(std::is_arithmetic<T>::value, "Requires either integral or floating point vector");

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
