cmake_minimum_required(VERSION 3.20)
project(CPU-Rasterizer LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} 
    src/main.c
    src/Application.c
    src/Framebuffer.c
    src/Display.c
    src/Model.c
    src/Processing.c
    src/Rasterizer.c
    src/Transform.c
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(external/sdl EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        SDL3::SDL3
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)