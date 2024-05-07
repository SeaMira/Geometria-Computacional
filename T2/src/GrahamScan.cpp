#include<iostream>
#include<cmath>
#include <stack>
#include <stdlib.h>
#include<Poligono/poligono.hpp>

Punto p0;

Punto nextToTop(std::stack<Punto> &S)
{
    Punto p = S.top();
    S.pop();
    Punto res = S.top();
    S.push(p);
    return res;
}

void swap(Punto &p1, Punto &p2) {
    Punto temp = p1;
    p1 = p2;
    p2 = temp;
}

int distSq(Punto p1, Punto p2)
{
    return (p1.GetX() - p2.GetX())*(p1.GetX() - p2.GetX()) +
          (p1.GetY() - p2.GetY())*(p1.GetY() - p2.GetY());
}
 

int orientation(Punto p, Punto q, Punto r)
{
    int val = (q.GetY() - p.GetY()) * (r.GetX() - q.GetX()) -
              (q.GetX() - p.GetX()) * (r.GetY() - q.GetY());
 
    if (val == 0) return 0;  // collinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}
 

int compare(const void *vp1, const void *vp2)
{
   Punto *p1 = (Punto *)vp1;
   Punto *p2 = (Punto *)vp2;
 
   int o = orientation(p0, *p1, *p2);
   if (o == 0)
     return (distSq(p0, *p2) >= distSq(p0, *p1))? -1 : 1;
 
   return (o == 2)? -1: 1;
}

Poligono grahamScan(Punto[] puntos) {

    int n = sizeof(puntos)/sizeof(Punto);
    float ymin = puntos[0].GetY(), min = 0;
    for (int i = 1; i < n; i++) {
        float y = puntos[i].GetY();

        // Pick the bottom-most or choose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y &&
            puntos[i].GetX() < puntos[min].GetX()))
        ymin = puntos[i].GetY(), min = i;
    }

    // Place the bottom-most point at first position
    swap(puntos[0], puntos[min]);

    p0 = puntos[0];
    qsort(&puntos[1], n-1, sizeof(Punto), compare);


    int m = 1; // Initialize size of modified array
    for (int i=1; i<n; i++) {
        
        while (i < n-1 && orientation(p0, puntos[i],
                                    puntos[i+1]) == 0)
            i++;

        puntos[m] = puntos[i];
        m++;  // Update size of modified array
    }

    if (m < 3) return;

    std::stack<Punto> S;
    S.push(puntos[0]);
    S.push(puntos[1]);
    S.push(puntos[2]);

    for (int i = 3; i < m; i++) {
        while (S.size()>1 && orientation(nextToTop(S), S.top(), puntos[i]) != 2)
            S.pop();
        S.push(puntos[i]);
    }

    std::vector<Punto<float>> vec;
    while (!S.empty()) {
        Punto p = S.top();
        vec.push_back(p);
        std::cout << "(" << p.GetX() << ", " << p.GetY() <<")" << std::endl;
        S.pop();
    }
    Poligono pol(vec);
    return pol;
}