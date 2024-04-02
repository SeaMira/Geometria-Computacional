#include <gtest/gtest.h>
#include <Vector/vector.hpp>
#include <cmath>
//Ver clase en:
//https://google.github.io/googletest/primer.html

class VectorClass2 : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
   Vector<int> v1{0, 0};
   Vector<float> v2{2.5f, 2.5f};
   Vector<int> v3{0, 1};
   Vector<float> v4{2.5f, 0.0f};
   
//    public:
//     VectorClass2() : p1(0, 0), p2(2.5f, 2.5f), p3(0, 1), p4(2.5f, 0.0f) {}

   void SetUp() override {
      // Code here will be called immediately after the constructor (right
      // before each test).
    //    p1 = Vector<int>(0,0);
    //    p2 = Vector<float>((float)2.5, (float)2.5);
    //    p3 = Vector<int>(0,1);
    //    p4 = Vector<float>((float)2.5, (float)0.0);
   }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
     
  }

};

TEST_F(VectorClass2, TestGetters) {
    EXPECT_EQ(v1.GetX(), 0);
    EXPECT_EQ(v1.GetY(), 0);
}

TEST_F(VectorClass2, TestSetters) {
    v1.SetX(1); v1.SetY(1);
    EXPECT_EQ(v1.GetX(), 1);
    EXPECT_EQ(v1.GetY(), 1);

}

// Demonstrate some basic assertions.
TEST_F(VectorClass2, TestVectorSize) {
    EXPECT_EQ(v1.Size(), 0);
    EXPECT_EQ(v2.Size(), sqrt(pow(v2.GetX(), 2) + pow(v2.GetY(), 2)));
}

TEST_F(VectorClass2, TestPointProduct) {
    EXPECT_EQ(v1.PointProduct(v3), 0);
    EXPECT_EQ(v2.PointProduct(v4), pow(2.5f, 2));
}

TEST_F(VectorClass2, TestCrossProduct) {
    EXPECT_EQ(v1.CrossProduct(v3), 0);
    EXPECT_EQ(v2.CrossProduct(v4), -pow(2.5f, 2));
}

TEST_F(VectorClass2, TestAddingVectors) {
    EXPECT_EQ(v1 + v3, Vector<int>(0, 1));
    EXPECT_EQ(v2 + v4, Vector<float>(5.0f, 2.5f));
}

TEST_F(VectorClass2, TestEquivalence) {
    EXPECT_TRUE(v1 == v1);
    EXPECT_FALSE(v2 == v4);
}

TEST_F(VectorClass2, TestScalarMult) {
    EXPECT_EQ(v3*3, Vector<int>(0, 3));
    EXPECT_EQ(v4*2, Vector<float>(5.0f, 0.0f));
}

TEST_F(VectorClass2, TestOutputOperator) {
    std::stringstream ss, ss2;
    ss << v1;
    EXPECT_EQ(ss.str(), "(0 , 0)");
    ss2 << v2;
    EXPECT_EQ(ss2.str(), "(2.5 , 2.5)");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}