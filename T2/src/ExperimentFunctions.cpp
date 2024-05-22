#include<Poligono/poligono.hpp>
#include <cfloat>
#include <cstdlib> 
#include <ctime> 
#include <cmath> 

Punto<float>* nPointList(int n, float rnge) {
    srand(time(0));
    Punto<float>* p = new Punto<float>[n];
    for (int i = 0; i < n; i++) {
        float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/rnge));
        float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/rnge));
        p[i] = Punto<float>(r, r2);
    }
    return p;
}


Punto<float>* pointsInRCirc(int n, float r) {
    srand(time(0));
    Punto<float>* p = new Punto<float>[n];
    for (int i = 0; i < n; i++) {
        float sign = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(2.0f*r))) - r;
        float y = std::sqrt(std::pow(r,2) - std::pow(x,2));
        if (sign >= 0.5) y*= -1;
        p[i] = Punto<float>(x, y);
    }
    return p;
}

Punto<float>* concatPointsArray(Punto<float>* p1, int s1, Punto<float>* p2, int s2) {
    Punto<float>* p = new Punto<float>[s1+s2];
    for (int i = 0; i < s1; i++) {
        p[i] = p1[i];
    }
    for (int j = 0; j < s2; j++) {
        p[s1+j] = p2[j];
    }
    return p;
}

Punto<float>* pcntConvexFromNPoints(int n, float pcnt, float rnge) {
    srand(time(0));
    int m = (int) (pcnt*(float)n);
    int k = n - m; 

    float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/rnge));

    Punto<float>* convexPoints = pointsInRCirc(m, r);
    Punto<float>* interiorPoints = nPointList(k, r);
 
    return concatPointsArray(convexPoints, m, interiorPoints, k);
}

bool equalPolygons(Poligono<float> p1, Poligono<float> p2) {
    int n = p1.GetPointsAmount();
    int m = p2.GetPointsAmount();
    bool equal = true;
    for (int i = 0; i< n; i++) {
        if (!p2.PointIsIn(p1[i])) {
            std::cout << p1[i] << i << " p1" << std::endl;
            equal = false;
        }
    }
    // std::cout << "Puntos de p1 están en p2" << std::endl;
    for (int j  = 0; j< m; j++) {
        if (!p1.PointIsIn(p2[j])) {
            std::cout << p2[j] << j << " p2" << std::endl;
            equal = false;
        }
    }
    // std::cout << "Puntos de p2 están en p1" << std::endl;
    return equal;

}


