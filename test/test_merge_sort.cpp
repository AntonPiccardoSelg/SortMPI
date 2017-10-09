#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "merge_sort.h"

namespace {
    // For each program we can initialize and finalize MPI
    // only once. Hence we cannot easily place these settings
    // in the test. This is for now an easy workaround.
    boost::mpi::environment env;
    boost::mpi::communicator world;
}


using namespace SortMPI;


TEST(MergeSortTest, WhenInputEmptyThenNothingIsSorted) {
// GIVEN an empty input vector
std::vector<int> input;


// WHEN applying it to merge sort
MergeSort sorter(world);
sorter.sort(input);

// THEN the vector is still empty.
ASSERT_EQ(input.size(), 0u);
}


TEST(MergeSortTest, WhenInputHasOnlyOneEntryNothingIsSorted) {
// GIVEN an input vector with one entry
std::vector<double> input = {1.4};

// WHEN applying it to merge sort
MergeSort sorter(world);
sorter.sort(input);

// THEN the vector has the same one element.
ASSERT_EQ(input.size(), 1u);
ASSERT_EQ(input[0], 1.4);
}


TEST(MergeSortTest, WhenEvenNumberOfElementsTheyGetSorted) {
// GIVEN an input vector with an even number of elements
std::vector<int> input = {3, 6, 12, 76, 2, 44, 23, 11};

// WHEN applying it to merge sort
MergeSort sorter(world);
sorter.sort(input);

// THEN the vector has its elements sorted
ASSERT_EQ(input.size(), 8u);
ASSERT_EQ(input[0], 2);
ASSERT_EQ(input[1], 3);
ASSERT_EQ(input[2], 6);
ASSERT_EQ(input[3], 11);
ASSERT_EQ(input[4], 12);
ASSERT_EQ(input[5], 23);
ASSERT_EQ(input[6], 44);
ASSERT_EQ(input[7], 76);
}


TEST(MergeSortTest, WhenOddNumberOfElementsTheyGetSorted) {
// GIVEN an input vector with an odd number of elements
std::vector<int> input = {3, 6, 12, 76, 2, 44,  11};

// WHEN applying it to merge sort
MergeSort sorter(world);
sorter.sort(input);

// THEN the vector has its elements sorted
ASSERT_EQ(input.size(), 7u);
ASSERT_EQ(input[0], 2);
ASSERT_EQ(input[1], 3);
ASSERT_EQ(input[2], 6);
ASSERT_EQ(input[3], 11);
ASSERT_EQ(input[4], 12);
ASSERT_EQ(input[5], 44);
ASSERT_EQ(input[6], 76);
}


TEST(MergeSortTest, WhenReverseSortedGetsSorted) {
// GIVEN an input vector which is reverse sorted
std::vector<int> input = {5, 4, 3, 2, 1};

// WHEN applying it to merge sort
MergeSort sorter(world);
sorter.sort(input);

// THEN the vector has its elements sorted
ASSERT_EQ(input.size(), 5u);
ASSERT_EQ(input[0], 1);
ASSERT_EQ(input[1], 2);
ASSERT_EQ(input[2], 3);
ASSERT_EQ(input[3], 4);
ASSERT_EQ(input[4], 5);
}