configure_file(config.h.in config.h @ONLY)

set(source_files
    nopayloadclient.cpp
    resthandler.cpp
    fakewrapper.cpp
    realwrapper.cpp
    payload.cpp
    moment.cpp
    iov.cpp
#    payloadiov.cpp
    plhandler.cpp
    config.cpp
    cache.cpp
    cli.cpp
)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(nopayloadclient ${source_files})

target_link_libraries(nopayloadclient PUBLIC CURL::libcurl)

if (UNIX AND NOT APPLE)
    target_link_libraries(nopayloadclient PUBLIC stdc++fs)
endif (UNIX AND NOT APPLE)

set(INCLUDE_DIR_NLOHMANN_JSON "${CMAKE_SOURCE_DIR}/contrib" CACHE STRING "Description of MY_VAR")
message(STATUS "INCLUDE_DIR_NLOHMANN_JSON: [${INCLUDE_DIR_NLOHMANN_JSON}]")

target_include_directories(nopayloadclient
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${INCLUDE_DIR_NLOHMANN_JSON}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS nopayloadclient DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

add_executable(cli_npc cli.cpp)
target_link_libraries(cli_npc nopayloadclient ${_std_cxx_filesystem_lib})
target_include_directories(cli_npc PRIVATE ${CMAKE_SOURCE_DIR}/include ${INCLUDE_DIR_NLOHMANN_JSON})
install(TARGETS cli_npc DESTINATION bin)

#target_compile_options(nopayloadclient PRIVATE -Wall -Wextra -Wpedantic -Werror)
