if(EXISTS "D:/Users/Escritorio/U/Otoño 2024 - 9no semestre/Geometria-Computacional/T2/build/test/GrahamScanTests[1]_tests.cmake")
  include("D:/Users/Escritorio/U/Otoño 2024 - 9no semestre/Geometria-Computacional/T2/build/test/GrahamScanTests[1]_tests.cmake")
else()
  add_test(GrahamScanTests_NOT_BUILT GrahamScanTests_NOT_BUILT)
endif()