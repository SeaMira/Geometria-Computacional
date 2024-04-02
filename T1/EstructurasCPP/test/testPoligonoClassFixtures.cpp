#include <gtest/gtest.h>
#include <Poligono/poligono.hpp>
#include <vector>
#include <cmath>
//Ver clase en:
//https://google.github.io/googletest/primer.html

class PoligonoClass2 : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
   Poligono<int> pol1;
   Poligono<float> pol2;
   Poligono<int> pol3;
   Poligono<float> pol4;
   
//    public:
    PoligonoClass2() 
       : pol1({Punto<int>(0,0), Punto<int>(1,0), Punto<int>(1,1), Punto<int>(0,1)}),
         pol2({Punto<float>(0.0f,0.0f), Punto<float>(1.1f,0), Punto<float>(1.1f,1.1f), Punto<float>(0,1.1f)}),
         pol3({Punto<int>(0,0), Punto<int>(0,1), Punto<int>(1,1), Punto<int>(1,0)}),
         pol4({Punto<float>(0.0f,0.0f), Punto<float>(0,1.1f), Punto<float>(1.1f,1.1f), Punto<float>(1.1f,0.0f)}) {}

   void SetUp() override {
      // Code here will be called immediately after the constructor (right
      // before each test).
    //    p1 = Poligono<int>(0,0);
    //    p2 = Poligono<float>((float)2.5, (float)2.5);
    //    p3 = Poligono<int>(0,1);
    //    p4 = Poligono<float>((float)2.5, (float)0.0);
   }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
     
  }

};

TEST_F(PoligonoClass2, TestGetPointsAmount) {
    EXPECT_EQ(pol1.GetPointsAmount(), 4);
    EXPECT_EQ(pol2.GetPointsAmount(), 4);
}

TEST_F(PoligonoClass2, TestOperatorSelector) {
    EXPECT_EQ(pol1[0], Punto<int>(0,0));
    EXPECT_EQ(pol2[0], Punto<float>(0.0f,0.0f));
    EXPECT_THROW(pol1[-1], std::out_of_range);
    EXPECT_THROW(pol1[4], std::out_of_range);

}

// Demonstrate some basic assertions.
TEST_F(PoligonoClass2, TestIsCounterclockwise) {
    EXPECT_TRUE(pol1.isCounterclockwise());
    EXPECT_FALSE(pol3.isCounterclockwise());
    EXPECT_TRUE(pol2.isCounterclockwise());
    EXPECT_FALSE(pol4.isCounterclockwise());
}

TEST_F(PoligonoClass2, TestOutputOperator) {
    std::stringstream ss, ss2;
    ss << pol1;
    EXPECT_EQ(ss.str(), "[(0, 0),(1, 0),(1, 1),(0, 1)]");
    ss2 << pol2;
    EXPECT_EQ(ss2.str(), "[(0, 0),(1.1, 0),(1.1, 1.1),(0, 1.1)]");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}