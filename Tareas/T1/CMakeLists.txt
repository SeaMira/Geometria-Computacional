cmake_minimum_required(VERSION 3.16.3)
project(structures)
set(CMAKE_CXX_STANDARD 17)

enable_testing()
include(CTest)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(test)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} punto vector poligono)