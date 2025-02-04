cmake_minimum_required(VERSION 3.13)

project(codecrafters-kafka)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/include/*.hpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(yaml-cpp REQUIRED)

add_executable(kafka ${SOURCE_FILES})

target_link_libraries(kafka PRIVATE yaml-cpp::yaml-cpp)

