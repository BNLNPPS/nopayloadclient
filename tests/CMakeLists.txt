include(FetchContent)
FetchContent_Declare(
    catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.6  # Update this to the version you need
)

FetchContent_MakeAvailable(catch)
message(STATUS "Catch2 downloaded to: ${catch_SOURCE_DIR}")

if ((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "13") OR
    (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "9.1"))
  set(_std_cxx_filesystem_lib "stdc++fs")
endif()

#add_executable(tests test_main.cpp)
#target_include_directories(tests PRIVATE ${catch_SOURCE_DIR}/single_include ${CMAKE_SOURCE_DIR}/include)
#target_link_libraries(tests Catch2::Catch2 nopayloadclient)
#list(APPEND CMAKE_MODULE_PATH ${catch_SOURCE_DIR}/contrib)
#include(Catch)
#catch_discover_tests(tests)

add_executable(test_nopayloadclient test_nopayloadclient.cpp)
target_link_libraries(test_nopayloadclient nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(test_nopayloadclient PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_nopayloadclient COMMAND $<TARGET_FILE:test_nopayloadclient>)

add_test(NAME test_cli COMMAND ${CMAKE_SOURCE_DIR}/tests/test_cli.sh $<TARGET_FILE:cli_npc> ${CMAKE_SOURCE_DIR}/config/default.json)

#add_executable(test_iov test_iov.cpp)
#target_link_libraries(test_iov nopayloadclient ${_std_cxx_filesystem_lib})
#target_include_directories(test_iov PRIVATE ${CMAKE_SOURCE_DIR}/include)
#add_test(NAME test_iov COMMAND $<TARGET_FILE:test_iov>)

#add_executable(test_cache test_cache.cpp)
#target_link_libraries(test_cache nopayloadclient ${_std_cxx_filesystem_lib})
#target_include_directories(test_cache PRIVATE ${CMAKE_SOURCE_DIR}/include)
#add_test(NAME test_cache COMMAND $<TARGET_FILE:test_cache>)

#add_executable(test_payload test_payload.cpp)
#target_link_libraries(test_payload nopayloadclient ${_std_cxx_filesystem_lib})
#target_include_directories(test_payload PRIVATE ${CMAKE_SOURCE_DIR}/include)
#add_test(NAME test_payload COMMAND $<TARGET_FILE:test_payload>)

#add_executable(test_backend test_backend.cpp)
#add_executable(test_plhandler test_plhandler.cpp)
#add_executable(test_get test_get.cpp)
#add_executable(test_config test_config.cpp)
#add_executable(test_size test_size.cpp)
#add_executable(test_cache test_cache.cpp)
#add_executable(test_exception test_exception.cpp)

#target_link_libraries(test_backend nopayloadclient ${_std_cxx_filesystem_lib})
#target_link_libraries(test_plhandler nopayloadclient ${_std_cxx_filesystem_lib})
#target_link_libraries(test_get nopayloadclient ${_std_cxx_filesystem_lib})
#target_link_libraries(test_config nopayloadclient ${_std_cxx_filesystem_lib})
#target_link_libraries(test_size nopayloadclient ${_std_cxx_filesystem_lib})
#target_link_libraries(test_cache nopayloadclient ${_std_cxx_filesystem_lib})
#target_link_libraries(test_exception nopayloadclient ${_std_cxx_filesystem_lib})

#target_include_directories(test_backend PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(test_plhandler PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(test_get PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(test_config PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(test_size PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(test_cache PRIVATE ${CMAKE_SOURCE_DIR}/include)
#target_include_directories(test_exception PRIVATE ${CMAKE_SOURCE_DIR}/include)

#add_test(NAME test_backend COMMAND ./test_backend)
#add_test(NAME test_plhandler COMMAND ./test_plhandler)
#add_test(NAME test_get COMMAND ./test_get)
#add_test(NAME test_config COMMAND ./test_config)
#add_test(NAME test_size COMMAND ./test_size)
#add_test(NAME test_cache COMMAND ./test_cache)
#add_test(NAME test_exception COMMAND ./test_exception)
