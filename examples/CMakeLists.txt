add_executable(cli_get cli_get.cpp)
target_link_libraries(cli_get nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(cli_get PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(check_size check_size.cpp)
target_link_libraries(check_size nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(check_size PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(cli_npc cli_npc.cpp)
target_link_libraries(cli_npc nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(cli_npc PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS cli_npc DESTINATION bin)