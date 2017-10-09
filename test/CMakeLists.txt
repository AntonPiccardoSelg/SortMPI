include_directories(../inc)

# --------------------------
# REGISTER TESTS
#--------------------------
function(add_sort_test TEST_NAME TEST_SOURCE)
    get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_TARGET} ${TEST_SOURCE})
    target_link_libraries(${TEST_TARGET} gtest gmock_main)
    add_test(${TEST_NAME} ${TEST_TARGET})
endfunction()

add_sort_test("sort.merge_sort" test_merge_sort.cpp)
