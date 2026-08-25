cmake_minimum_required(VERSION 3.20)
project(CPU Rasterizer)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(#{PROJECT_NAME} src/main.c)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
