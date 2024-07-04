#include<iostream>
#include"Disk/disk.hpp"

int main(int argc, char const *argv[])
{
    float radius = std::stof(argv[1]);
    int points = std::stoi(argv[2]);
    float pcnt = std::stof(argv[3]);
    std::cout << "radius " << radius << " points " << points << " pcnt " << pcnt << std::endl;

    DiskTriangulation diskT(radius, points, pcnt);
    diskT.draw_delaunay();
    diskT.write_delaunay_off("delaunay_disk.off");
    diskT.write_voronoi_off("voronoi_disk.off");
    diskT.write_delaunay_node_ele("delaunay_disk.node", "delaunay_disk.ele");
    diskT.write_voronoi_node_ele("voronoi_disk.node", "voronoi_disk.ele");

    return 0;
}

//https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html