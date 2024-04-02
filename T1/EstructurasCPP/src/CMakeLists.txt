set(PUNTO_HPP_LIST "${PROJECT_SOURCE_DIR}/include/Punto/punto.hpp")
set(VECTOR_HPP_LIST "${PROJECT_SOURCE_DIR}/include/Vector/vector.hpp")
set(POLIGONO_HPP_LIST "${PROJECT_SOURCE_DIR}/include/Vector/poligono.hpp")
set(PUNTO_CPP_LIST "punto.cpp")
set(VECTOR_CPP_LIST "vector.cpp")
set(POLIGONO_CPP_LIST "poligono.cpp")

add_library(punto  ${PUNTO_CPP_LIST} ${HEADER_LIST})
add_library(vector  ${VECTOR_CPP_LIST} ${HEADER_LIST})
add_library(poligono  ${POLIGONO_CPP_LIST} ${HEADER_LIST})

target_include_directories(punto PUBLIC ../include)
target_include_directories(vector PUBLIC ../include)
target_include_directories(poligono PUBLIC ../include)