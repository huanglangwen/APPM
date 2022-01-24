cmake_minimum_required(VERSION 3.13)

project(appm)

#include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindUMFPACK.cmake")

add_executable(appm)
set_property(TARGET appm PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -DNDEBUG -DMKL_ILP64 -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

#add_compile_definitions(EIGEN_USE_MKL_ALL)
find_package (HDF5 REQUIRED COMPONENTS C CXX HL)
#find_package(MKL CONFIG REQUIRED)
#message(STATUS "${MKL_IMPORTED_TARGETS}")
#set(BLA_VENDOR OpenBLAS)
#find_package(BLAS)

target_include_directories(appm PRIVATE $ENV{EIGEN_ROOT}/include/eigen3)
#target_include_directories(appm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(appm PRIVATE ${HDF5_INCLUDE_DIR})
#target_include_directories(appm PRIVATE ${UMFPACK_INCLUDE_DIRS})
#target_include_directories(appm PRIVATE "/usr/include/suitesparse")
#target_include_directories(appm PRIVATE "/usr/include/superlu")

#FIND_LIBRARY(UMFPACK_LIBRARIES NAMES umfpack libumfpack UMFPACK libUMFPACK
#               PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
#               PATHS "/usr/lib/x86_64-linux-gnu/"
#               NO_DEFAULT_PATH)

#FIND_LIBRARY(SUPERLU_LIBRARIES NAMES superlu libsuperlu SUPERLU libSUPERLU
#               PATH_SUFFIXES ${CMAKE_LIBRARY_ARCHITECTURE}
#               PATHS "/usr/lib/x86_64-linux-gnu/"
#               NO_DEFAULT_PATH)
include_directories($ENV{MKLROOT}/include)
link_directories($ENV{MKLROOT}/lib/intel64)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC)
target_sources(appm PRIVATE ${SRC})
#target_compile_options(appm PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
#target_include_directories(appm PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
#target_link_libraries(appm PUBLIC $<LINK_ONLY:MKL::MKL>)
target_link_libraries(appm PRIVATE  ${HDF5_LIBRARIES})
target_link_libraries(appm PRIVATE mkl_intel_ilp64 mkl_intel_thread mkl_core iomp5)
#target_link_libraries(appm PRIVATE ${UMFPACK_LIBRARIES})
#target_link_libraries(appm PRIVATE ${SUPERLU_LIBRARIES})
#target_link_libraries(appm PRIVATE BLAS::BLAS)

# set(PROJECT_BINARY_DIR "D:/ytw/Thesis/build")
message(STATUS "Project binary dir: ${PROJECT_BINARY_DIR}")
message(STATUS "HDF5 root: ${HDF5_ROOT}")
message(STATUS "HDF5 version: ${HDF5_VERSION}")
message(STATUS "HDF5 include dir: ${HDF5_INCLUDE_DIR}")
message(STATUS "HDF5 CXX lib: ${HDF5_LIBRARIES}")
