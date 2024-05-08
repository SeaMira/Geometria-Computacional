#include <gtest/gtest.h>
#include "../src/GiftWrapping.cpp"
#include <vector>
#include <cmath>

class GiftWrappingClass2 : public ::testing::Test {
 protected:
  
   Punto<float> p1{0.0f, 0.0f};
   Punto<float> p2{2.5f, 0.0f};
   Punto<float> p3{1.0f, 1.0f};
   Punto<float> p4{0.0f, 2.5f};
   Punto<float> p5{2.5f, 2.5f};
   
   Punto<float> puntos1[5] = {p1, p2, p3, p4, p5};
   Poligono<float> pol;
   GiftWrappingClass2():
   pol({p1, p2, p4, p5}) {}

};

TEST_F(GiftWrappingClass2, TestGetters) {
    Poligono<float> pol2 = giftWrapping(puntos1, 5);
    std::cout << pol2;
    ASSERT_TRUE(pol2.PointIsIn(p1));
    ASSERT_TRUE(pol2.PointIsIn(p2));
    ASSERT_TRUE(!pol2.PointIsIn(p3));
    ASSERT_TRUE(pol2.PointIsIn(p4));
    ASSERT_TRUE(pol2.PointIsIn(p5));
    
}