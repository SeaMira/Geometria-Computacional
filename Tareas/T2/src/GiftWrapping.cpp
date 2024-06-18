#include<iostream>
#include <algorithm>
#include<cmath>
#include <chrono>
#include <fstream>
#include"ExperimentFunctions.cpp"


struct TimesGW {
  long find_min, operations;
  long total() { return find_min+operations; }
};

float Max(float x, float y) {
    return x > y ? x : y;
}

float Min(float x, float y) {
    return x > y ? y : x;
}

int orientation(Punto<float> p, Punto<float> q, Punto<float> r)
{
    float val = (q.GetY() - p.GetY()) * (r.GetX() - q.GetX()) -
              (q.GetX() - p.GetX()) * (r.GetY() - q.GetY());
 
    if (val == 0.0f) return 0;  // collinear
    return (val > 0.0f)? 1: 2; // clock or counterclock wise
}

bool onSegment(Punto<float> p, Punto<float> q, Punto<float> r) 
{ 
    return (q.GetX() <= Max(p.GetX(), r.GetX()) && q.GetX() >= Min(p.GetX(), r.GetX()) && 
            q.GetY() <= Max(p.GetY(), r.GetY()) && q.GetY() >= Min(p.GetY(), r.GetY()));
       
} 


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

TimesGW t_gw;

Poligono<float> giftWrappingRegister(Punto<float> puntos[], int n, char *filename) {
    using std::chrono::microseconds;
   
    std::ofstream gw_file(filename, std::ios::app); // Open the file for writing
    if (!gw_file.is_open()) {
        std::cerr << "Failed to open file\n";
    }

    if (n  < 3) {
        // error
    }

    auto t_start = std::chrono::high_resolution_clock::now();
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
    auto t_end = std::chrono::high_resolution_clock::now();
    t_gw.find_min = std::chrono::duration_cast<microseconds>(t_end - t_start).count();
    Poligono<float> pol({});

    int p = min_x_idx, q ;

    t_start = std::chrono::high_resolution_clock::now();
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
    t_end = std::chrono::high_resolution_clock::now();
    t_gw.operations = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    gw_file << n << "," << t_gw.find_min << "," << t_gw.operations << "," << t_gw.total() << "," << pol.GetPointsAmount() <<  "\n";
    gw_file.close();
    return pol;
}