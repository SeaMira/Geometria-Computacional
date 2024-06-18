#include<iostream>
#include"Poligono/poligono.hpp"
// #include"Punto/punto.hpp"
// #include"Vector/vector.hpp"

int main(int argc, char const *argv[])
{
    Punto<int> p1{0, 0}, p2{0, 1}, p3{1, 0}, p4{2, 3};
    Punto<double> p5{0.0, 0.0}, p6{0.0, 1.5}, p7{1.5, 2.5}, p8{2.5, 3.5};

    std::cout << "Get x p1: " << p1.GetX() << std::endl;
    std::cout << "Get y p1: " << p1.GetY() << std::endl;
    std::cout << "Get x p8: " << p8.GetX() << std::endl;
    std::cout << "Get y p8: " << p8.GetY() << std::endl;

    p1.SetX(1);
    p1.SetY(1);
    p8.SetX(1.5);
    p8.SetY(1.5);
    std::cout << "Set x p1 from 0 to 1: " << p1.GetX() << std::endl;
    std::cout << "Set y p1 from 0 to 1: " << p1.GetY() << std::endl;
    std::cout << "Set x p8 from 2.5 to 1.5: " << p1.GetX() << std::endl;
    std::cout << "Set y p8 from 3.5 to 1.5: " << p1.GetY() << std::endl;

    double dist1 = p1.DistToPoint(p3);
    std::cout << "Distance from p1 (1, 1) to p3 (0, 1): " << dist1 << std::endl;
    double dist2 = p5.DistToPoint(p6);
    std::cout << "Distance from p5 (0.0, 0.0) to p6 (0.0, 1.5): " << dist2 << std::endl;

    std::cout << "p1 "<< p1 <<" equal to p2 (0, 1)?: " << (p1 == p2) << std::endl;
    std::cout << "Setting p2 to (1, 1) " << std::endl;
    p2.SetX(1);
    std::cout << "p1 "<< p1 <<" equal to p2 "<< p2 <<": " << (p1 == p2) << std::endl;
    
    std::cout << "p8 (1.5, 1.5) equal to p6 (0.0, 1.5)?: " << (p8 == p6) << std::endl;
    std::cout << "Setting p6 to (1.5, 1.5) " << std::endl;
    p6.SetX(1.5);
    std::cout << "p8 "<< p8 <<" equal to p6 "<< p6 <<": " << (p8 == p6) << std::endl;

    std::cout << "Printing points: " << std::endl;
    std::cout << p1 << std::endl;
    std::cout << p4 << std::endl;
    std::cout << p5 << std::endl;
    std::cout << p7 << std::endl;

    std::cout << std::endl;

    Vector<int> v1{0, 0}, v2{0,1}, v3{1,1}, v4{1,0};
    Vector<float> v5{0.0, 0.0}, v6{0.0,1.5}, v7{1.5,1.5}, v8{1.5,0.0};
    std::cout << "Get x v1: " << v1.GetX() << std::endl;
    std::cout << "Get y v1: " << v1.GetY() << std::endl;
    std::cout << "Get x v8: " << v8.GetX() << std::endl;
    std::cout << "Get y v8: " << v8.GetY() << std::endl;

    v1.SetX(1);
    v1.SetY(1);
    v8.SetX(2.5);
    v8.SetY(1.5);
    std::cout << "Set x v1 from 0 to 1: " << v1.GetX() << std::endl;
    std::cout << "Set y v1 from 0 to 1: " << v1.GetY() << std::endl;
    std::cout << "Set x v8 from 1.5 to 2.5: " << v1.GetX() << std::endl;
    std::cout << "Set y v8 from 0.0 to 1.5: " << v1.GetY() << std::endl;

    std::cout << "v2 " << v2 << " size: " << v2.Size() << std::endl;
    std::cout << "point product between v2 " << v2 << " and v3 "<< v3 << ": " << v2.PointProduct(v3) << std::endl;
    std::cout << "Same as 0*1+1*1: " << 0*1+1*1 << std::endl;

    std::cout << "v6 " << v6 << " size: " << v6.Size() << std::endl;
    std::cout << "point product between v8 " << v8 << " and v7 "<< v7 << ": " << v8.PointProduct(v7) << std::endl;
    std::cout << "Same as 2.5*1.5+1.5*1.5: " << 2.5*1.5+1.5*1.5 << std::endl;

    std::cout << "cross product between v2 " << v2 << " and v3 "<< v3 << ": " << v2.CrossProduct(v3) << std::endl;
    std::cout << "Same as 0*1-1*1: " << 0*1-1*1 << std::endl;

    std::cout << "cross product between v8 " << v8 << " and v7 "<< v7 << ": " << v8.CrossProduct(v7) << std::endl;
    std::cout << "Same as 2.5*1.5-1.5*1.5: " << 2.5*1.5-1.5*1.5 << std::endl;

    std::cout << "Adding v2 " << v2 << " and v3 "<< v3 << ": " << v2+v3 << std::endl;
    std::cout << "Same as {0+1, 1+1}" << std::endl;

    std::cout << "Adding v8 " << v8 << " and v7 "<< v7 << ": " << v8+v7 << std::endl;
    std::cout << "Same as {1.5+2.5, 1.5+1.5}" << std::endl;

    std::cout << "Set x v2 from 0 to 1: " << v2.GetX() << std::endl;
    v2.SetX(1);
    std::cout << "equivalence between v2 " << v2 << " and v3 "<< v3 << ": " << (v2==v3) << std::endl;

    std::cout << "Set x v8 from 2.5 to 1.5: " << v1.GetX() << std::endl;
    v8.SetX(1.5);
    std::cout << "equivalence between v8 " << v8 << " and v7 "<< v7 << ": " << (v8==v7) << std::endl;

    std::cout << "Scalar multiplication to v2 by 3: " << v2*3 << std::endl;
    std::cout << "Scalar multiplication to v7 by 2: " << v7*2 << std::endl;


    Poligono<int> pol1({Punto<int>(0,0), Punto<int>(1,0), Punto<int>(1,1), Punto<int>(0,1)}),
    pol2({Punto<int>(0,0), Punto<int>(0,1), Punto<int>(1,1), Punto<int>(1,0)});
    Poligono<float> pol3({Punto<float>(0.0f,0.0f), Punto<float>(1.1f,0), Punto<float>(1.1f,1.1f), Punto<float>(0,1.1f)}),
    pol4({Punto<float>(0.0f,0.0f), Punto<float>(0,1.1f), Punto<float>(1.1f,1.1f), Punto<float>(1.1f,0.0f)});

    std::cout << "Showing polygons:" << std::endl;
    std::cout << "pol1: " << pol1 << std::endl;
    std::cout << "pol2: " << pol2 << std::endl;
    std::cout << "pol3: " << pol3 << std::endl;
    std::cout << "pol4: " << pol4 << std::endl;

    std::cout << "Amount of points of pol1: " << pol1.GetPointsAmount() << std::endl;
    std::cout << "Amount of points of pol3: " << pol3.GetPointsAmount() << std::endl;
    
    std::cout << "[] operator on pol1 for point 2: " << pol1[1] << std::endl;
    std::cout << "[] operator on pol3 for point 2: " << pol3[1] << std::endl;

    std::cout << "pol1 is given in counterclockwise order of points: " << pol1.isCounterclockwise() << std::endl;
    std::cout << "pol2 is given in counterclockwise order of points: " << pol2.isCounterclockwise() << std::endl;
    std::cout << "pol3 is given in counterclockwise order of points: " << pol3.isCounterclockwise() << std::endl;
    std::cout << "pol4 is given in counterclockwise order of points: " << pol4.isCounterclockwise() << std::endl;
    return 0;

}

