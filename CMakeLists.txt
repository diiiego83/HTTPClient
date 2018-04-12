cmake_minimum_required (VERSION 3.0)

project (httpclient)

set (CMAKE_CXX_STANDARD 11)

set(EXAMPLES_OUT_DIR ${CMAKE_BINARY_DIR}/bin/examples)
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
endif()
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/dist/httpclient)

add_library(httpclient SHARED src/HTTPClient.cc src/HTTPClientIMPL.cc)

add_executable(get examples/get.cc)
target_link_libraries(get httpclient curl)
set_target_properties(get PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUT_DIR})

add_executable(status examples/status.cc)
target_link_libraries(status httpclient curl)
set_target_properties(status PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXAMPLES_OUT_DIR})

install (TARGETS httpclient DESTINATION lib )
install (FILES  ${CMAKE_SOURCE_DIR}/src/HTTPClient.h DESTINATION include)