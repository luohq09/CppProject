cmake_minimum_required(VERSION 3.3)
project(cannon_proxy)

add_definitions(-O2 --std=c++11 -Wall -Wextra -Wno-deprecated-declarations -DNDEBUG)
# add_definitions(--std=c++11 -Wall -Wextra -Wno-deprecated-declarations)
include_directories("cannon/interface/gen-cpp")
include_directories("src/thread_pool")

set(SOURCE_FILES
        src/main.cpp
        src/proxy_server.cpp
        interface/gen-cpp/Cannon_Service.cpp
        interface/gen-cpp/proxy_constants.cpp
        interface/gen-cpp/proxy_types.cpp
        src/cannon_client_wrapper.cpp
        src/cannon_client_wrapper.h
        cannon/interface/gen-cpp/Cannon.cpp
        cannon/interface/gen-cpp/cannon_constants.cpp
        cannon/interface/gen-cpp/cannon_types.cpp
        src/model_files_reader.cpp
        src/model_files_reader.h)

add_executable(cannon_proxy ${SOURCE_FILES})
target_link_libraries(cannon_proxy thrift)
