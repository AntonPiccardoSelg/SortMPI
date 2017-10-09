cmake_minimum_required ( VERSION 3.5 )
project ( sort_mpi )

set ( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

# ---------------------------
# COMPILER FLAGS
# ---------------------------
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc /wd4521")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
            CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
            CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()


# ---------------------------
# Includes and sub dirs
# ---------------------------
include_directories(inc)
include ( BoostMPI )

# ---------------------------
# Target setup
# ---------------------------
set(TARGET_NAME sort_mpi)
add_executable ( ${TARGET_NAME} main.cpp)


# ---------
# MPI setup
# ---------
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries( ${TARGET_NAME}  ${MPI_LIBRARIES})
if(MPI_COMPILE_FLAGS)
    set_target_properties(${TARGET_NAME}  PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(${TARGET_NAME}  PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
target_link_libraries ( ${TARGET_NAME}  Boost::mpi )


# -----------------------
# TESTS
# -----------------------
option(MERGESORT_BUILD_TESTS "Build the merge sort tests" ON)
if(${MERGESORT_BUILD_TESTS})
    include(CTest)
    enable_testing()

    # Get GTest and GMock
    if (CMAKE_VERSION VERSION_LESS 3.5)
        set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
    else()
        set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
    endif()

    include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/DownloadProject.cmake)

    download_project(PROJ                googletest
            GIT_REPOSITORY      https://github.com/google/googletest.git
            GIT_TAG             master
            ${UPDATE_DISCONNECTED_IF_AVAILABLE}
            )

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

    # When using CMake 2.8.11 or later, header path dependencies
    # are automatically added to the gtest and gmock targets.
    # For earlier CMake versions, we have to explicitly add the
    # required directories to the header search path ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include"
                "${gmock_SOURCE_DIR}/include")
    endif()

    add_subdirectory(test)
endif()


# ------------------------------
# Avoid duplicate compiler flags
# ------------------------------
set(CONFIGURED_ONCE TRUE CACHE STRING "Flag for compiler flag setting")