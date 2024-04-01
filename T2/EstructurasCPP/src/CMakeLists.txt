set(HPP_LIST "${PROJECT_SOURCE_DIR}/include/Punto/punto.hpp")
set(CPP_LIST "punto.cpp")

add_library(punto  ${CPP_LIST} ${HEADER_LIST})
target_include_directories(punto PUBLIC ../include)