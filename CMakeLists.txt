cmake_minimum_required(VERSION 3.10.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)


add_executable(raytracer main.cpp glad.c stb_image.h)

target_include_directories(raytracer PRIVATE
    ${CMAKE_SOURCE_DIR}   # because glad.h is in the root directory
)
target_link_libraries(raytracer glfw glm::glm)

