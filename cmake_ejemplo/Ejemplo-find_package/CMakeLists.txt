cmake_minimum_required(VERSION 3.16.3)
project(TestCgal)
set(CMAKE_CXX_STANDARD 17)

add_executable(TestCgal main.cpp)

find_package(CGAL REQUIRED COMPONENTS Core)
target_link_libraries(TestCgal CGAL::CGAL CGAL::CGAL_Core)
