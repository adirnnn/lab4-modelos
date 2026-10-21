cmake_minimum_required(VERSION 3.24)
project(lab4_modelos LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# sdl2 se compila como libreria estatica para no tener que copiar dlls
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.10
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL2 glm)

add_executable(software_renderer
    src/main.cpp
    src/framebuffer.cpp
)

target_link_libraries(software_renderer PRIVATE SDL2::SDL2main SDL2::SDL2-static glm::glm)

if(MSVC)
    target_compile_options(software_renderer PRIVATE /W4 /utf-8)
else()
    target_compile_options(software_renderer PRIVATE -Wall -Wextra)
endif()
