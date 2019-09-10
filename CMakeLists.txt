cmake_minimum_required(VERSION 3.0)
project(test_large_hdf5_write)

# output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# write compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# compile flags
set(CMAKE_C_FLAGS_RELEASE "-O3 -march=native")

set(SRC
    src/save.c
    src/main.c
    )

add_executable(test_large_hdf5_write ${SRC})
set_property(TARGET test_large_hdf5_write PROPERTY C_STANDARD 99)

# MPI
find_package(MPI REQUIRED)
target_link_libraries(test_large_hdf5_write PRIVATE MPI::MPI_C)

# HDF5
find_package(hdf5 REQUIRED COMPONENTS C HL)
if (NOT HDF5_IS_PARALLEL)
    message(FATAL_ERROR "We require HDF5 with parallel support.")
endif()
target_include_directories(test_large_hdf5_write PRIVATE ${HDF5_INCLUDE_DIRS})
target_link_libraries(test_large_hdf5_write PRIVATE ${HDF5_C_LIBRARIES} ${HDF5_C_HL_LIBRARIES})
add_definitions(${HDF5_DEFINITIONS})
# if(NOT (${HDF5_C_HL_LIBRARIES} MATCHES "libhdf5_hl"))
    # # THIS IS A TEMPORARY HACK TO OVERCOME A BUG IN SOME VERSIONS OF CMAKE
    # list(GET HDF5_C_LIBRARIES 0 HDF5_LIB)
    # get_filename_component(HDF5_LIBDIR ${HDF5_LIB} DIRECTORY)
    # target_link_libraries(test_large_hdf5_write PRIVATE "${HDF5_LIBDIR}/libhdf5_hl.dylib")
# endif()

