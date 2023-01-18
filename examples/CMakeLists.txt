add_executable(cli_npc cli_npc.cpp)
target_link_libraries(cli_npc nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(cli_npc PRIVATE ${CMAKE_SOURCE_DIR}/include)
install(TARGETS cli_npc DESTINATION bin)