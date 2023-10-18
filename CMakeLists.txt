cmake_minimum_required(VERSION 3.20)

project(nopayloadclient VERSION 0.0.0 LANGUAGES CXX)

add_compile_definitions(_LIBCPP_NO_EXPERIMENTAL_DEPRECATION_WARNING_FILESYSTEM)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Set a debug postfix
set(CMAKE_DEBUG_POSTFIX "-dbg")

# Set installation destinations typical for *nix systems
include(GNUInstallDirs)
message(STATUS "CMAKE_INSTALL_PREFIX: [${CMAKE_INSTALL_PREFIX}]")

# Install files and export configs
include(CMakePackageConfigHelpers)
include(FetchContent)

# Set up external dependencies
find_package(CURL REQUIRED)
# Download external nlohmann_json if specified
option(USE_EXTERNAL_JSON "Use an external JSON library" OFF)
if(USE_EXTERNAL_JSON)
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
    FetchContent_MakeAvailable(json)
else()
    find_package(nlohmann_json REQUIRED)
endif()

add_subdirectory(src)

# Set up tests
option(BUILD_TESTING "" OFF)
if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(tests)

#    list(APPEND CMAKE_MODULE_PATH ${catch_SOURCE_DIR}/contrib)
#    include(Catch)
#    catch_discover_tests(tests)

endif()

