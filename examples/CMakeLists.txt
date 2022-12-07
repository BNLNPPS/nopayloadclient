add_executable(example_insert example_insert.cpp)
target_link_libraries(example_insert nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(example_insert PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(example_insert_short example_insert_short.cpp)
target_link_libraries(example_insert_short nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(example_insert_short PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(example_read example_read.cpp)
target_link_libraries(example_read nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(example_read PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(cli_get cli_get.cpp)
target_link_libraries(cli_get nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(cli_get PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(check_size check_size.cpp)
target_link_libraries(check_size nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(check_size PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(example_sphenix example_sphenix.cpp)
target_link_libraries(example_sphenix nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(example_sphenix PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(cli_sphenix cli_sphenix.cpp)
target_link_libraries(cli_sphenix nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(cli_sphenix PRIVATE ${CMAKE_SOURCE_DIR}/include)
