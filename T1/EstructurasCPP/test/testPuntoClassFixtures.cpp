#include <gtest/gtest.h>
#include <Punto/punto.hpp>
#include <cmath>
//Ver clase en:
//https://google.github.io/googletest/primer.html

class PuntoClass2 : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.
   Punto<int> p1{0, 0};
   Punto<float> p2{2.5f, 2.5f};
   Punto<int> p3{0, 1};
   Punto<float> p4{2.5f, 0.0f};
   
//    public:
//     PuntoClass2() : p1(0, 0), p2(2.5f, 2.5f), p3(0, 1), p4(2.5f, 0.0f) {}

   void SetUp() override {
      // Code here will be called immediately after the constructor (right
      // before each test).
    //    p1 = punto<int>(0,0);
    //    p2 = punto<float>((float)2.5, (float)2.5);
    //    p3 = punto<int>(0,1);
    //    p4 = punto<float>((float)2.5, (float)0.0);
   }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
     
  }
/*
    CircleClass2() {
        // You can do set-up work for each test here    
    }
    
  ~CircleClass2() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
     std::cout<<"Inicia Test"<<std::endl;
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
  */
};

TEST_F(PuntoClass2, TestGetters) {
    EXPECT_EQ(p1.GetX(), 0);
    EXPECT_EQ(p1.GetY(), 0);
    EXPECT_EQ(p1.ToString(), "(0, 0)");
}

TEST_F(PuntoClass2, TestSetters) {
    p1.SetX(1); p1.SetY(1);
    EXPECT_EQ(p1.GetX(), 1);
    EXPECT_EQ(p1.GetY(), 1);

}

// Demonstrate some basic assertions.
TEST_F(PuntoClass2, TestDistanceBetweenPoints) {
    EXPECT_EQ(p1.DistToPoint(p3), 1);
    EXPECT_EQ(p2.DistToPoint(p4), 2.5);
}

TEST_F(PuntoClass2, TestEquivalence) {
    EXPECT_TRUE(p1 == p1);
    EXPECT_FALSE(p2 == p4);
}

TEST_F(PuntoClass2, TestOutputOperator) {
    std::stringstream ss, ss2;
    ss << p1;
    EXPECT_EQ(ss.str(), "(0 , 0)");
    ss2 << p2;
    EXPECT_EQ(ss2.str(), "(2.5 , 2.5)");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}