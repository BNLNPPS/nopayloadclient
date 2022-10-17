add_executable(example example.cpp)
target_link_libraries(example nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(example PRIVATE ${CMAKE_SOURCE_DIR}/include)
