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
#include <set>
#include <random>
#include <stack>
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

// Creates a disk of radius r with a pointsA amount of points. A percentage (pcnt) of them are on the border and the rest are randomly inside.
CDT getCircleTriangulation(float r, int pointsA, float pcnt);

class Cropped_voronoi_from_delaunay {
    public:
        std::list<Segment_2> m_cropped_vd;
        Cropped_voronoi_from_delaunay(){}

        void crop_and_extract_segment(const Segment_2& rsl, float radius, int points, float pcnt);
        
        
};

class DiskTriangulation {
    private:
        float radius, pcnt;
        int points = 0;
        CDT disk;
        std::list<Segment_2> cropped_vd_disk;
        std::vector<std::vector<Point_2>> voronoi_segments;
        std::vector<Point_2> bd_points;

        std::unordered_map<CDT::Face_handle, std::pair<bool, Segment_2>> in_domain_map;
        boost::associative_property_map< std::unordered_map<CDT::Face_handle,std::pair<bool, Segment_2>>> in_domain;

    public:
        DiskTriangulation(float radius, int points, float pcnt);
        void compute_voronoi();
        void write_delaunay_off(const std::string& filename);
        void write_voronoi_off(const std::string& filename);
        void draw_delaunay();
        void getCircleTriangulation(CDT& t, float r, int pointsA, float pcnt);
        // std::vector<Point_2> extractValidSegments(std::vector<CDT::Face_handle>& pts);
};