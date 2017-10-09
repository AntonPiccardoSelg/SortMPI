#include "merge_sort.h"

template<typename T>
void print(std::vector<T>& toPrint) {
    for (const auto& element : toPrint) {
        std::cout << element << " ";
    }
    std::cout << "\n";
}


int main() {
    mpi::environment env;
    mpi::communicator world;

    std::vector<int> test = {2, 23, 2,1 ,54, 1, 4, 7, 134, 5,};
    MergeSort sorter(world);
    sorter.sort(test);

    if (world.rank() == 0) {
        std::cout << "OUTPUT\n";
        print(test);
    }

    return 0;
}