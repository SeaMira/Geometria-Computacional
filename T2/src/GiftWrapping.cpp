#include<iostream>
#include<cmath>
#include<Poligono/poligono.hpp>



float cross_product(Punto<float> p0, Punto<float> p1, Punto<float> p2) {
    return (p1.GetX() - p0.GetX())*(p2.GetY() - p1.GetY()) - (p1.GetY() - p0.GetY())*(p2.GetX() - p1.GetX());
}

Poligono<float> giftWrapping(Punto<float> puntos[], int n) {
    // Punto<float> * p = puntos;
    // int n = sizeof(puntos)/sizeof(Punto<float>);
    std::cout << n << std::endl;
    if (n  < 3) {
        // error
    }
    int min_x_idx = 0;
    float min_x = puntos[0].GetX();
    for (int i = 1; i < n; i++) {
        if (puntos[i].GetX() < min_x){
            min_x = puntos[i].GetX();
            min_x_idx = i;
        } 
    }

    Poligono<float> pol({puntos[min_x_idx]});

    int p = min_x_idx, q ;
    while (q != min_x_idx) {
        q = (p + 1) % n;

        int i = 0; 
        while (i < n) {
            if (i != p && i != q) {
                float crss_pdct = cross_product(puntos[p], puntos[q], puntos[i]);
                if (crss_pdct < 0) {
                    q = i;
                }
            }
            i++;
        } 
        pol.addPoint(puntos[q]);
        p = q;
        
    }
    return pol;
}