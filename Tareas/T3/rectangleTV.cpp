#include<iostream>
#include"Rectangle/rectangle.hpp"

int main(int argc, char const *argv[])
{
    float b_side = std::stof(argv[1]);
    float l_side = std::stof(argv[2]);
    int points = std::stoi(argv[3]);
    int bd_points = std::stoi(argv[4]);

    RectangleTriangulation recT(b_side, l_side, points, bd_points);
    recT.draw_delaunay();
    recT.write_delaunay_off("delaunay_rec.off");
    recT.write_voronoi_off("voronoi_rec.off");
    return 0;
}
