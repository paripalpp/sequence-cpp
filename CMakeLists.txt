cmake_minimum_required(VERSION 3.8)
project(sequence-cpp VERSION 0.1)

set(SEQUENCE_CPP_SOURCES
    src/sequence.cpp
    src/builder.cpp
)

add_library(sequence-cpp ${SEQUENCE_CPP_SOURCES})
target_include_directories(sequence-cpp PUBLIC include)
target_link_libraries(sequence-cpp yaml-cpp)
target_compile_features(sequence-cpp PRIVATE cxx_std_20)
