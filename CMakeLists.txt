cmake_minimum_required(VERSION 2.8)
project(web_project)

message("Building on system: ${CMAKE_SYSTEM} (${CMAKE_SYSTEM_PROCESSOR})")
message("Using cmake generator: ${CMAKE_GENERATOR}")

include(ExternalProject)

include(${PROJECT_SOURCE_DIR}/thirdparty/onion.cmake)

add_executable(main ${PROJECT_SOURCE_DIR}/main.c)

target_link_libraries(main onion)

add_dependencies(main ${ONION_PREFIX})
