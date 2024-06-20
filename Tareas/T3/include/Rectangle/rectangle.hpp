#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <CGAL/mark_domain_in_triangulation.h>

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
#include <vector>
#include <unordered_map>
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




void getRectTriangulation(float b_side, float l_side, int points, int bd_points, CDT* t, std::unordered_map<CDT::Face_handle, bool>& in_domain_map);

class RectangleTriangulation {
    private:
        float b_side, l_side;
        int points = 0;

        std::list<Segment_2> cropped_vd_rec;
        std::unordered_map<CDT::Face_handle, bool> in_domain_map;
        boost::associative_property_map< std::unordered_map<CDT::Face_handle,bool>> in_domain;

    public:
        CDT rect;

        RectangleTriangulation(float b_side, float l_side, int points, int bd_points);
        void compute_voronoi();
        void draw_delaunay();
        void write_delaunay_off(const std::string& filename);
        void write_voronoi_off(const std::string& filename);
};