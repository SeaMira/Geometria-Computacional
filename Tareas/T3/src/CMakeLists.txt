set(DISK_HPP_LIST "${PROJECT_SOURCE_DIR}/include/Disk/disk.hpp")
set(DISK_CPP_LIST "disk.cpp")
add_library(disk  ${DISK_CPP_LIST} ${DISK_HPP_LIST})
target_include_directories(disk PUBLIC ../include)

set(RECT_CPP_LIST "rectangle.cpp")
set(RECT_HPP_LIST "${PROJECT_SOURCE_DIR}/include/Rectangle/rectangle.hpp")
add_library(rectangle  ${RECT_CPP_LIST} ${RECT_HPP_LIST})
target_include_directories(rectangle PUBLIC ../include)


set(PIKACHU_CPP_LIST "pikachu.cpp")
set(PIKACHU_HPP_LIST "${PROJECT_SOURCE_DIR}/include/Pikachu/pikachu.hpp")
add_library(pikachu  ${PIKACHU_CPP_LIST} ${PIKACHU_HPP_LIST})
target_include_directories(pikachu PUBLIC ../include)