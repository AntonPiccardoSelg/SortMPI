# Sort MPI

MPI implementations for sort algorithms. Sor far only *Mergesort* has been implemented.
Other "divide-and-conquer" algorithms can be added in a similar manner. The project is only meant to be a learning
exercise.


### To build

To build with ninja:
```
mkdir build_sort_mpi
cd build_sort_mpi
cmake -GNinja PATH_TO_SOURCE
ninja
```

### To run the tests

To run this on `m` processes:
```
mpirun -n m PATH_TO_BUILD/test/test_merge_sort
```

### To run the examples

To run this on `m` processes:
```
mpirun -n m PATH_TO_BUILD/example/merge_sort_example
```


