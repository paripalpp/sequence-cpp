cmake_minimum_required(VERSION 3.8)
project(sequence-cpp VERSION 0.3)

find_package(yaml-cpp REQUIRED)

add_library(sequence-cpp SHARED
    src/sequence.cpp
    src/builder.cpp
)

target_include_directories(sequence-cpp PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(sequence-cpp PUBLIC yaml-cpp)
target_compile_features(sequence-cpp PUBLIC cxx_std_20)

install(TARGETS sequence-cpp
    EXPORT sequence-cpp-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT sequence-cpp-targets
    FILE sequence-cpp-config.cmake
    NAMESPACE sequence-cpp::
    DESTINATION lib/cmake/sequence-cpp
)
