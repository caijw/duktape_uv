cmake_minimum_required(VERSION 2.8.9)

project (duktape_uv)

set(ROOT_DIR  ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${ROOT_DIR}/src
    ${ROOT_DIR}/libuv-v1.44.2/include
)

include(duktape.cmake)

add_subdirectory(libuv-v1.44.2)

# add_library(duktape_uv)

add_executable(duktapeuv src/main.c)

target_link_libraries(duktapeuv duktape uv)