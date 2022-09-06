if ((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "13") OR
    (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "9.1"))
  set(_std_cxx_filesystem_lib "stdc++fs")
endif()

add_executable(test_nopayloadclient test_nopayloadclient.cpp)
add_executable(test_backend test_backend.cpp)
target_link_libraries(test_nopayloadclient nopayloadclient ${_std_cxx_filesystem_lib})
target_link_libraries(test_backend nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(test_nopayloadclient PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_backend PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME test_nopayloadclient COMMAND ./test_nopayloadclient)
add_test(NAME test_backend COMMAND ./test_backend)
