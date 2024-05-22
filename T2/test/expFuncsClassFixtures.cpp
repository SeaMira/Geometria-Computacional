#include <gtest/gtest.h>
#include "../src/ExperimentFunctions.cpp"
#include <vector>
#include <cmath>

class ExpFuncsClass2 : public ::testing::Test {};


TEST_F(ExpFuncsClass2, ExpFuncs) {

    Punto<float>* p1 = nPointList(100, FLT_MAX);
    for (int i = 0; i < 100; i++) {
        std::cout << p1[i] << std::endl;
    }
    
    
}