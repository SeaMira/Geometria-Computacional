if(EXISTS "D:/Users/Escritorio/U/Otoño 2024 - 9no semestre/Geometria-Computacional/T1/EstructurasCPP/build/test/VectorTests[1]_tests.cmake")
  include("D:/Users/Escritorio/U/Otoño 2024 - 9no semestre/Geometria-Computacional/T1/EstructurasCPP/build/test/VectorTests[1]_tests.cmake")
else()
  add_test(VectorTests_NOT_BUILT VectorTests_NOT_BUILT)
endif()