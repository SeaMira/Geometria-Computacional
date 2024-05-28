#include <gtest/gtest.h>
#include "../src/ExperimentFunctions.cpp"
#include <vector>
#include <cmath>

class ExpFuncsClass2 : public ::testing::Test {};


TEST_F(ExpFuncsClass2, ExpFuncsnPointLists) {

    Punto<float>* p1 = nPointList(100, sqrt(FLT_MAX));
    for (int i = 0; i < 100; i++) {
        std::cout << p1[i] << std::endl;
    }
    
    
}


TEST_F(ExpFuncsClass2, ExpFuncspointsInRCirc) {

    Punto<float>* p = pointsInRCirc(100, sqrt(FLT_MAX));
    for (int i = 0; i < 100; i++) {
        bool statement1 = FLT_MAX-p[i].GetY()*p[i].GetY() <= p[i].GetX()*p[i].GetX() + 0.1;
        bool statement2 = FLT_MAX-p[i].GetY()*p[i].GetY() >= p[i].GetX()*p[i].GetX() - 0.1;
        ASSERT_TRUE(statement1 || statement2);
    }
    
}

TEST_F(ExpFuncsClass2, ExpFuncsconcatPointsArray) {
    Punto<float> p1{0.0f, 0.0f};
    Punto<float> p2{2.5f, 0.0f};
    Punto<float> p3{1.0f, 1.0f};
    Punto<float> p4{0.0f, 2.5f};
    Punto<float> p5{2.5f, 2.5f};

    Punto<float> puntos1[3] = {p1, p2, p3};
    Punto<float> puntos2[2] = {p4, p5};
    Punto<float>* puntos3 = concatPointsArray(puntos1, 3, puntos2, 2);
    ASSERT_EQ(puntos3[0], p1);
    ASSERT_EQ(puntos3[1], p2);
    ASSERT_EQ(puntos3[2], p3);
    ASSERT_EQ(puntos3[3], p4);
    ASSERT_EQ(puntos3[4], p5);
}

TEST_F(ExpFuncsClass2, ExpFuncsequalPolygons) {

    Punto<float> p1{0.0f, 0.0f};
    Punto<float> p2{2.5f, 0.0f};
    Punto<float> p3{1.0f, 1.0f};
    Punto<float> p4{0.0f, 2.5f};
    Punto<float> p5{2.5f, 2.5f};

    Poligono<float> pol({p1, p2, p3, p4, p5});
    Poligono<float> pol2({p1, p2, p3, p4, p5});
    ASSERT_TRUE(equalPolygons(pol,pol2));
    
}