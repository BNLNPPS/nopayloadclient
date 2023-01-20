cmake_minimum_required(VERSION 3.20)

project(nopayloadclient VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a debug postfix
set(CMAKE_DEBUG_POSTFIX "-dbg")

# Set installation destinations typical for *nix systems
include(GNUInstallDirs)
if (DEFINED ENV{NPC_INSTALL_DIR})
    message(STATUS "Detected user defined NPC_INSTALL_DIR...")
    set(CMAKE_INSTALL_PREFIX "$ENV{NPC_INSTALL_DIR}")
else()
    message(STATUS "NPC_INSTALL_DIR not set. Using default...")
endif()
message(STATUS "CMAKE_INSTALL_PREFIX: [${CMAKE_INSTALL_PREFIX}]")

# Set up external dependencies
find_package(CURL REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)

# Install files and export configs
include(CMakePackageConfigHelpers)

# Set up tests
option(BUILD_TESTING "" OFF)
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
