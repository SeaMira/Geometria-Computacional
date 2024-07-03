#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <CGAL/mark_domain_in_triangulation.h>

#include <CGAL/intersections.h>
#include <CGAL/Object.h>
#include <CGAL/enum.h>

#include <cassert>
#include <cfloat>
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
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

class VCP {  
    public:           
        bool is_out;
        Point_2 circ;         
        Segment_2 border; 
        VCP(bool is_out, Point_2 circ, Segment_2 border): is_out(is_out), circ(circ), border(border) {} 
        bool operator==(const VCP vcp) const {
            return (this->is_out == vcp.is_out) && (this->circ == vcp.circ) && (this->border == vcp.border);
        } 
};       


class PikachuTriangulation {
    private:
        float b_side, l_side;
        int points = 0;
        Point_2 body_tail_sp = Point_2(10448, -12928);
        Point_2 mouth_sp =  Point_2(6193, -8525);
        Point_2 left_eye_sp =  Point_2(5422, -6776);
        Point_2 right_eye_sp = Point_2(9555, -7906);
        CDT pikachu;


        std::unordered_map<CDT::Face_handle, bool> in_domain_map;
        boost::associative_property_map< std::unordered_map<CDT::Face_handle,bool>> in_domain;
        std::list<Segment_2> cropped_vd_rec;
        std::vector<std::vector<Point_2>> voronoi_segments;
        std::vector<Segment_2> border;

    public:

        PikachuTriangulation(const std::string& filename);
        void compute_voronoi();
        void draw_delaunay();
        void write_delaunay_off(const std::string& filename);
        void write_voronoi_off(const std::string& filename);
        std::vector<Point_2> extractValidSegments(std::vector<Point_2>& vor_cell, Point_2 voronoi_sector);
};