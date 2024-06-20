cmake_minimum_required(VERSION 3.16.3)
project(delunayToVoronoi)
set(CMAKE_CXX_STANDARD 17)

# Buscar CGAL y componentes opcionales
find_package(CGAL REQUIRED)

# Agregar directorios
# add_subdirectory(extern)
add_subdirectory(src)
# add_subdirectory(test)

# Desactivar pruebas si no son necesarias
# enable_testing()

#CGAL_Qt5 is needed for the drawing.
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)
if(CGAL_Qt5_FOUND)
  #required to use basic_viewer
  add_definitions(-DCGAL_USE_BASIC_VIEWER -DQT_NO_KEYWORDS)
endif()
if ( CGAL_FOUND )
  #create the executable of the application
  create_single_source_cgal_program("diskTV.cpp")
  create_single_source_cgal_program("rectangleTV.cpp")
  create_single_source_cgal_program("pikachuTV.cpp")
  if(CGAL_Qt5_FOUND )
    #link it with the required CGAL libraries
    target_link_libraries(disk PUBLIC CGAL::CGAL_Qt5 CGAL::CGAL_Basic_viewer)
    target_link_libraries(diskTV PUBLIC disk)
    target_link_libraries(rectangle PUBLIC CGAL::CGAL_Qt5 CGAL::CGAL_Basic_viewer)
    target_link_libraries(rectangleTV PUBLIC rectangle)
    target_link_libraries(pikachu PUBLIC CGAL::CGAL_Qt5 CGAL::CGAL_Basic_viewer)
    target_link_libraries(pikachuTV PUBLIC pikachu)
  endif()
else()
    message(STATUS "This program requires the CGAL library, and will not be compiled.")
endif()