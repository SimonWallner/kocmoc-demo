# root build file for the KOCMOC demo

cmake_minimum_required(VERSION 2.8)
project(kocmoc CXX)

# add_subdirectory(external/gl3w)
# add_subdirectory(external/glm)
add_subdirectory(external/opencollada)
add_subdirectory(src)

