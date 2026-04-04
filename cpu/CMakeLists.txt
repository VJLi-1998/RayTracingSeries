cmake_minimum_required(VERSION 3.10)
project(raytracing_series_cpu LANGUAGES CXX)

# Optional: set a modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create executable from main.cpp
add_executable(raytracer main.cpp)

# Optional: enable warnings
if (MSVC)
    target_compile_options(raytracer PRIVATE /W4 /permissive-)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic)
endif()
