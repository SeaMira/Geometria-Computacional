#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/draw_triangulation_2.h>

#include <CGAL/intersections.h>
#include <CGAL/Object.h>

#include <cassert>
#include <cfloat>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <unordered_map>
#include <unordered_set> 
#include <algorithm> 
#include <boost/property_map/property_map.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel       K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Ray_2 Ray_2;
typedef K::Line_2 Line_2;

typedef CGAL::Triangulation_vertex_base_2<K>                      Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K>            Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>               TDS;

typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;


class DiskTriangulation {
    private:
        float radius, pcnt;
        int points = 0;
        CDT disk;

        std::vector<std::vector<Point_2>> voronoi_segments;
        std::vector<Point_2> bd_points;


    public:
        DiskTriangulation(float radius, int points, float pcnt);
        void compute_voronoi();
        void write_delaunay_off(const std::string& filename);
        void write_voronoi_off(const std::string& filename);
        void draw_delaunay();
        void getCircleTriangulation(CDT& t, float r, int pointsA, float pcnt);
};