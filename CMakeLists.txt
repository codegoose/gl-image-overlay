cmake_minimum_required(VERSION 3.13)
project(id)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

add_subdirectory(nanovg)

add_executable(id
    "main.c"
    "stb_implementation.c"
)

target_link_libraries(id PUBLIC nanovg)
target_link_libraries(id PUBLIC ${CONAN_LIBS})