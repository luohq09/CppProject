cmake_minimum_required(VERSION 3.0)

project(cannon)
find_package(Boost 1.55.0 REQUIRED COMPONENTS filesystem system)

add_definitions(-O2 --std=c++11 -Wall -Wextra -Wno-deprecated-declarations -DNDEBUG)

# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "lib")

include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/cannon")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/hash_table")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/interface/gen-cpp")

link_libraries(glog pthread thrift gflags)

set(CANNONGENCPP_SOURCES
    interface/gen-cpp/Cannon.cpp
    interface/gen-cpp/cannon_constants.cpp
    interface/gen-cpp/cannon_types.cpp
    interface/gen-cpp/Cannon_server.skeleton.cpp
)

add_library(cannongencpp STATIC ${CANNONGENCPP_SOURCES})
target_link_libraries(cannongencpp thrift)

set(CANNON_SOURCES
    src/cannon/storage.cpp
)

add_library(cannonlib STATIC ${CANNON_SOURCES})

add_executable(CannonServer src/cannon/cannon_server.cpp)
target_link_libraries(CannonServer cannongencpp)
target_link_libraries(CannonServer thrift)
target_link_libraries(CannonServer cannonlib)
target_link_libraries(CannonServer ${Boost_LIBRARIES})
target_link_libraries(CannonServer gflags)
target_link_libraries(CannonServer glog)