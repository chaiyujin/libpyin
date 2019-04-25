cmake_minimum_required(VERSION 3.10)
project(pyin)

link_directories(external/libgvps/build/)

add_definitions("-DFP_TYPE=float")
set(cmake_c_flags "-Ofast -std=c99 -Wall -fPIC $(CFLAGSEXT)")

add_library(pyin common.h math-funcs.h pyin.h math-funcs.c yin.c pyin.c)
set_target_properties(pyin PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(pyin gvps m)

add_executable(pyin-test external/matlabfunctions.c test/test.c)
target_link_libraries(pyin-test pyin)

add_subdirectory(external/pybind11)
pybind11_add_module(pyin11 pybind.cpp)
target_link_libraries(pyin11 PRIVATE "$<$<NOT:$<CONFIG:Debug>>:pyin>")

