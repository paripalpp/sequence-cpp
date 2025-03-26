cmake_minimum_required(VERSION 3.8)
project(sequence-cpp VERSION 0.2)

find_package(yaml-cpp REQUIRED)

add_library(sequence-cpp 
    src/sequence.cpp
    src/builder.cpp
)

target_include_directories(sequence-cpp PUBLIC include)
target_link_libraries(sequence-cpp PUBLIC yaml-cpp)
target_compile_features(sequence-cpp PUBLIC cxx_std_20)
