cmake_minimum_required(VERSION 3.20)
project(untitled2)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled2 main.cpp)

# Nukopijuoja duomenu faila i aplanka, is kurio CLion paleidzia programa
configure_file(${CMAKE_SOURCE_DIR}/uzsakymas.txt ${CMAKE_BINARY_DIR}/uzsakymas.txt COPYONLY)
