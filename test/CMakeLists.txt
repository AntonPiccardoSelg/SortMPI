include_directories(../inc)

# --------------------------
# REGISTER TESTS
#--------------------------
function(add_sort_test TEST_NAME TEST_SOURCE)
    get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
    include_directories(${MPI_INCLUDE_PATH})
    add_executable(${TEST_TARGET} ${TEST_SOURCE})
    target_link_libraries(${TEST_TARGET} gtest gmock_main)
    target_link_libraries( ${TEST_TARGET}  ${MPI_LIBRARIES})

    if(MPI_COMPILE_FLAGS)
        set_target_properties(${TEST_TARGET}  PROPERTIES
                COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    endif()

    if(MPI_LINK_FLAGS)
        set_target_properties(${TEST_TARGET}  PROPERTIES
                LINK_FLAGS "${MPI_LINK_FLAGS}")
    endif()

    target_link_libraries ( ${TEST_TARGET}  Boost::mpi )

    add_test(${TEST_NAME} ${TEST_TARGET})
endfunction()

add_sort_test("sort.merge_sort" test_merge_sort.cpp)
