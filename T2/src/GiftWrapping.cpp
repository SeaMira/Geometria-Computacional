#include<iostream>
#include<cmath>
#include<Poligono/poligono.hpp>



float cross_product(Punto p0, Punto p1, Punto p2) {
    return (p2.GetX() - p1.GetX())*(p0.GetY() - p1.GetY()) - (p2.GetY() - p1.GetY())*(p0.GetX() - p1.GetX());
}

Poligono giftWrapping(Punto[] puntos) {
    Punto * p = puntos;
    int n = sizeof(puntos)/sizeof(Punto);
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

    Poligono pol({puntos[min_x_idx]});

    int p = min_x_idx, q = (min_x_idx + 1) % n ;
    while (q != min_x_idx) {

        int i = 0; 
        while (i < n) {
            if (i != p && i != q) {
                float crss_pdct = cross_product(puntos[p], puntos[q], puntos[i]);
                if (crss_pdct < 0) {
                    q = i;
                    break;
                }
            }
            i++;
        }
        if (i == n) {
            pol.addPoint(puntos[q]);
            p = q;
            q = (q + 1) % n;
        }
    }
    return pol;
}