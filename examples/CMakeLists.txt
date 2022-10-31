add_executable(example_insert example_insert.cpp)
target_link_libraries(example_insert nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(example_insert PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(example_read example_read.cpp)
target_link_libraries(example_read nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(example_read PRIVATE ${CMAKE_SOURCE_DIR}/include)
