#include<iostream>
#include"Disk/disk.hpp"

int main(int argc, char const *argv[])
{
    float radius = std::stof(argv[1]);
    int points = std::stoi(argv[2]);
    float pcnt = std::stof(argv[3]);

    DiskTriangulation diskT(radius, points, pcnt);
    diskT.draw_delaunay();
    diskT.write_delaunay_off("delaunay_disk.off");
    diskT.write_voronoi_off("voronoi_disk.off");
    return 0;
}

//https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html