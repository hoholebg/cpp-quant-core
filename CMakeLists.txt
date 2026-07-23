cmake_minimum_required(VERSION 3.14)
project(cpp_quant_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -ffast-math")

include_directories(include)

add_library(quant_core STATIC src/option_pricer.cpp src/orderbook.cpp)

add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE quant_core)
