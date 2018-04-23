cmake_minimum_required (VERSION 3.0)

project (httpclient)

set (CMAKE_CXX_STANDARD 11)

################################################################
# BUILD SHARED LIBRARY #########################################
################################################################

file(GLOB sources src/*.cc)
add_library(httpclient SHARED ${sources})

################################################################
# BUILD EXAMPLES ###############################################
################################################################

file(GLOB sources test/examples/*.cc)
foreach(source ${sources})
    get_filename_component(source ${source} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" program_name ${source})
    add_executable(${program_name} test/examples/${source})
    target_link_libraries(${program_name} httpclient curl)
    set_target_properties(${program_name} 
                          PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                          ${CMAKE_BINARY_DIR}/bin/examples)
endforeach()

################################################################
# BUILD UNIT TEST ##############################################
################################################################

enable_testing()
add_subdirectory(test/googletest/googletest EXCLUDE_FROM_ALL)
include_directories(test/googletest/googletest/include)
add_executable(utx_httpclient test/utest/utx_httpclient.cc)
target_link_libraries(utx_httpclient httpclient curl gtest)
add_test(utx_httpclient_test utx_httpclient)

################################################################
# INSTALL ######################################################
################################################################

if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
endif()
set(CMAKE_INSTALL_PREFIX 
    ${CMAKE_INSTALL_PREFIX}/dist/httpclient)
install (TARGETS httpclient DESTINATION lib)
install (FILES  ${CMAKE_SOURCE_DIR}/src/HTTPClient.h 
         DESTINATION include)