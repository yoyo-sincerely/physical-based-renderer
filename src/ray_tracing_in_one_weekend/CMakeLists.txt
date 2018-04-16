cmake_minimum_required(VERSION 3.7)
project(raytracer_in_one_weekend)

set(CMAKE_CXX_STANDARD 11)

set(FILES 
camera.hpp
hitable_list.hpp
hitable.hpp
material.hpp
ray.hpp
sphere.hpp
vec3.hpp
rand.hpp
main.cpp
)
add_executable(raytracer_in_one_weekend ${FILES})