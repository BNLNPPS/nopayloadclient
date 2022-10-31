configure_file(config.h.in config.h @ONLY)

set(source_files
    interface.cpp
    backend.cpp
    curlwrapper.cpp
    plmover.cpp
    config.cpp
)


add_library(nopayloadclient ${source_files})
target_link_libraries(nopayloadclient PUBLIC CURL::libcurl)
target_link_libraries(nopayloadclient PUBLIC stdc++fs)

target_include_directories(nopayloadclient
    INTERFACE
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/contrib
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(nopayloadclient PROPERTIES
    VERSION ${PROJECT_VERSION}
    POSITION_INDEPENDENT_CODE ON)

install(TARGETS nopayloadclient EXPORT nopayloadclientTargets
    RUNTIME   DESTINATION ${NOPAYLOADCLIENT_RUNTIME_INSTALL_DIR}
    LIBRARY   DESTINATION ${NOPAYLOADCLIENT_LIBRARY_INSTALL_DIR}
    ARCHIVE   DESTINATION ${NOPAYLOADCLIENT_ARCHIVE_INSTALL_DIR}
    FRAMEWORK DESTINATION ${NOPAYLOADCLIENT_FRAMEWORK_INSTALL_DIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/nopayloadclient/ DESTINATION ${NOPAYLOADCLIENT_INCLUDE_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h DESTINATION ${NOPAYLOADCLIENT_INCLUDE_INSTALL_DIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/version.h DESTINATION ${NOPAYLOADCLIENT_INCLUDE_INSTALL_DIR})
