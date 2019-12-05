cmake_minimum_required(VERSION 3.15)
project(pea2)

set(CMAKE_CXX_STANDARD 14)

add_executable(pea2 main.cpp Graph.cpp Graph.h TabuSearch.cpp TabuSearch.h SimulatedAnnealing.cpp SimulatedAnnealing.h)