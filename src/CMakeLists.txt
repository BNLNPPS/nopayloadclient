configure_file(config.h.in config.h @ONLY)

set(source_files
    nopayloadclient.cpp
    backend.cpp
    curlwrapper.cpp
    payload.cpp
    plmover.cpp
    config.cpp
)

add_library(nopayloadclient ${source_files})

target_link_libraries(nopayloadclient PUBLIC CURL::libcurl)

if (UNIX AND NOT APPLE)
    target_link_libraries(nopayloadclient PUBLIC stdc++fs)
endif (UNIX AND NOT APPLE)

target_include_directories(nopayloadclient
    INTERFACE
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(nopayloadclient PROPERTIES
    VERSION ${PROJECT_VERSION}
    POSITION_INDEPENDENT_CODE ON)

install(TARGETS nopayloadclient DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
