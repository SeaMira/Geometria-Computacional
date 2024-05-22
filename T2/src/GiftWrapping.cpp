#include<iostream>
#include <algorithm>
#include<cmath>
#include"ExperimentFunctions.cpp"


float Max(float x, float y) {
    return x > y ? x : y;
}

float Min(float x, float y) {
    return x > y ? y : x;
}

int orientation(Punto<float> p, Punto<float> q, Punto<float> r)
{
    int val = (q.GetY() - p.GetY()) * (r.GetX() - q.GetX()) -
              (q.GetX() - p.GetX()) * (r.GetY() - q.GetY());
 
    if (val == 0) return 0;  // collinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool onSegment(Punto<float> p, Punto<float> q, Punto<float> r) 
{ 
    return (q.GetX() <= Max(p.GetX(), r.GetX()) && q.GetX() >= Min(p.GetX(), r.GetX()) && 
            q.GetY() <= Max(p.GetY(), r.GetY()) && q.GetY() >= Min(p.GetY(), r.GetY()));
       
} 

// float cross_product(Punto<float> p0, Punto<float> p1, Punto<float> p2) {
//     return (p1.GetX() - p0.GetX())*(p2.GetY() - p1.GetY()) - (p1.GetY() - p0.GetY())*(p2.GetX() - p1.GetX());
// }

Poligono<float> giftWrapping(Punto<float> puntos[], int n) {
    // Punto<float> * p = puntos;
    // int n = sizeof(puntos)/sizeof(Punto<float>);
    if (n  < 3) {
        // error
    }
    int min_x_idx = 0;
    float min_x = puntos[0].GetX();

    for (int i = 1; i < n; i++) {
        if (puntos[i].GetX() < min_x){
            min_x = puntos[i].GetX();
            min_x_idx = i;
        } else if (puntos[i].GetX() == min_x && puntos[i].GetY() < puntos[min_x_idx].GetY()) {
		    min_x_idx = i; 
            min_x = puntos[i].GetX();
        }
    }

    Poligono<float> pol({});

    int p = min_x_idx, q ;

    do
    {
        pol.addPoint(puntos[p]);
 
        q = (p+1)%n;
        for (int i = 0; i < n; i++)
        {
            if (orientation(puntos[p], puntos[i], puntos[q]) == 2) q = i;
            if (p != i && orientation(puntos[p], puntos[i], puntos[q]) == 0 && onSegment(puntos[p], puntos[q], puntos[i])) q = i; 
		} 
 
        p = q;
 
    } while (p != min_x_idx);

    return pol;
}