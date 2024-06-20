#include<iostream>
#include"Pikachu/pikachu.hpp"

int main(int argc, char const *argv[]) {

    PikachuTriangulation pik("pikachu.off");
    pik.draw_delaunay();
    pik.write_delaunay_off("delaunay_pikachu.off");
    // diskT.write_voronoi_off("voronoi.off");
    return 0;
}
