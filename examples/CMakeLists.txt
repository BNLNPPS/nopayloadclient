cmake_minimum_required(VERSION 3.20)

project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nopayloadclient REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example nopayloadclient::nopayloadclient)

install(TARGETS example)
