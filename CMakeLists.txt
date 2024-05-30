cmake_minimum_required(VERSION 3.29.2)

project(SKI-Interpreter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(SkiInterpreter ski/main.cc)
