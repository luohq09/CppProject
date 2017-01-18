cmake_minimum_required(VERSION 3.0)

#generate version.h
execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        COMMAND git rev-parse --abbrev-ref HEAD
)
string(STRIP "${GIT_BRANCH}" GIT_BRANCH)
execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_ID
        COMMAND git rev-parse --short HEAD
)
string(STRIP "${GIT_COMMIT_ID}" GIT_COMMIT_ID)
execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE BUILD_DATE
        COMMAND date +%Y%m%d-%H%M%S
)
string(STRIP "${BUILD_DATE}" BUILD_DATE)
set(VERSION_STRING "char *version = \"branch: ${GIT_BRANCH}\\ncommit: ${GIT_COMMIT_ID}\\ndate: ${BUILD_DATE}\"\;" )
execute_process(
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/cannon
        OUTPUT_FILE version.h
        COMMAND echo ${VERSION_STRING}
)

project(cannon)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.62.0 REQUIRED COMPONENTS filesystem system)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

add_definitions(-O2 --std=c++11 -Wall -Wextra -Wno-deprecated-declarations -DNDEBUG)
#add_definitions(--std=c++11)


set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")


# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

link_directories("/usr/local/lib")
#link_directories("/home/work/3rdparty/gflags/lib")
#link_directories("/home/work/3rdparty/gtest/lib")
include_directories("/usr/local/include")
#include_directories("/home/work/3rdparty/gflags/include")
#include_directories("/home/work/3rdparty/gtest/include")
include_directories(SYSTEM "${Boost_INCLUDE_DIRS}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/cannon")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/interface/gen-cpp")

link_libraries(gtest glog gflags pthread)

set(CANNONGENCPP_SOURCES
    interface/gen-cpp/Cannon.cpp
    interface/gen-cpp/cannon_constants.cpp
    interface/gen-cpp/cannon_types.cpp
    interface/gen-cpp/Cannon_server.skeleton.cpp
)

add_library(cannongencpp STATIC ${CANNONGENCPP_SOURCES})
target_link_libraries(cannongencpp thrift)

set(CANNON_SOURCES
        src/cannon/kv_schema.cpp
        src/cannon/kv_schema_type.cpp
        src/cannon/immutable_hash_map.cpp
        src/cannon/storage.cpp)

add_library(cannonlib STATIC ${CANNON_SOURCES})

add_executable(CannonServer src/cannon/cannon_server.cpp)
target_link_libraries(CannonServer cannongencpp)
target_link_libraries(CannonServer pthread)
target_link_libraries(CannonServer thrift)
target_link_libraries(CannonServer cannonlib)
target_link_libraries(CannonServer ${Boost_LIBRARIES})
target_link_libraries(CannonServer gflags)
target_link_libraries(CannonServer glog)

add_subdirectory(src/cannon/test)
