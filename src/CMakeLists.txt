configure_file(config.h.in config.h @ONLY)

set(source_files
    nopayloadclient.cpp
    backend.cpp
    curlwrapper.cpp
    payload.cpp
    plhandler.cpp
    config.cpp
)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

add_library(nopayloadclient ${source_files})

target_link_libraries(nopayloadclient PUBLIC CURL::libcurl)

if (UNIX AND NOT APPLE)
    target_link_libraries(nopayloadclient PUBLIC stdc++fs)
endif (UNIX AND NOT APPLE)

target_include_directories(nopayloadclient
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS nopayloadclient DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
