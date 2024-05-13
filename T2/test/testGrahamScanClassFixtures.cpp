#include <gtest/gtest.h>
#include "../src/GrahamScan.cpp"
#include <vector>
#include <cmath>

class GrahamScanClass2 : public ::testing::Test {
 protected:
  
   Punto<float> p1{0.0f, 0.0f};
   Punto<float> p2{2.5f, 0.0f};
   Punto<float> p3{1.0f, 1.0f};
   Punto<float> p4{0.0f, 2.5f};
   Punto<float> p5{2.5f, 2.5f};
   
   Punto<float> puntos1[5] = {p1, p2, p3, p4, p5};
   Poligono<float> pol;
   GrahamScanClass2():
   pol({p1, p2, p4, p5}) {}

};

TEST_F(GrahamScanClass2, GrahamScan5Points) {
    Poligono<float> pol2 = grahamScan(puntos1, 5);
    // std::cout << pol2;
    ASSERT_TRUE(pol2.PointIsIn(p1));
    ASSERT_TRUE(pol2.PointIsIn(p2));
    ASSERT_TRUE(!pol2.PointIsIn(p3));
    ASSERT_TRUE(pol2.PointIsIn(p4));
    ASSERT_TRUE(pol2.PointIsIn(p5));
    
}



TEST_F(GrahamScanClass2, GrahamScan22Points) {

    Punto<float> puntos2[22] = {Punto<float>(0.0f, 0.0f), Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f),Punto<float>(0.0f, 0.0f)};
    for (int i = 0; i < 10; i+=2) {
        puntos2[i] = Punto<float>((float)i, 0.0f);
        puntos2[i+1] = Punto<float>((float)(i+1), 1.0f);
        puntos2[i+11] = Punto<float>((float)i, 5.0f);
        puntos2[i+12] = Punto<float>((float)(i+1), 4.0f);
    }
    puntos2[10] = Punto<float>(10.0f, 0.0f); puntos2[21] = Punto<float>(10.0f, 5.0f);

    Poligono<float> pol3 = grahamScan(puntos2, 22);
    std::cout << pol3;

    ASSERT_TRUE(pol3.PointIsIn(Punto<float>(0.0f, 0.0f)));
    ASSERT_TRUE(pol3.PointIsIn(Punto<float>(10.0f, 0.0f)));
    ASSERT_TRUE(pol3.PointIsIn(Punto<float>(10.0f, 5.0f)));
    ASSERT_TRUE(pol3.PointIsIn(Punto<float>(0.0f, 5.0f)));
    
    
    
}