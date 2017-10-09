include_directories(../inc)

set(EXAMPLES
    merge_sort_example
)

# -------------------
# Build each example.
# -------------------
foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)

    # ---------
    # MPI setup
    # ---------
    include_directories(${MPI_INCLUDE_PATH})
    target_link_libraries( ${EXAMPLE}  ${MPI_LIBRARIES})
    if(MPI_COMPILE_FLAGS)
        set_target_properties(${EXAMPLE}  PROPERTIES
                COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
    endif()

    if(MPI_LINK_FLAGS)
        set_target_properties(${EXAMPLE}  PROPERTIES
                LINK_FLAGS "${MPI_LINK_FLAGS}")
    endif()
    target_link_libraries ( ${EXAMPLE}  Boost::mpi )
endforeach()