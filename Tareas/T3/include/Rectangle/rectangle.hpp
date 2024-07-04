// Incluye los encabezados de CGAL necesarios
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/draw_triangulation_2.h>
#include <CGAL/mark_domain_in_triangulation.h>
#include <CGAL/intersections.h>
#include <CGAL/Object.h>

// Incluye otros encabezados de C++ y Boost
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


// Definición de tipos utilizando el kernel de construcciones exactas e inexactas de CGAL
typedef CGAL::Exact_predicates_inexact_constructions_kernel       K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Ray_2 Ray_2;
typedef K::Line_2 Line_2;
// Definición de tipos para la triangulación de Delaunay con restricciones
typedef CGAL::Triangulation_vertex_base_2<K>                      Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K>            Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>               TDS;

typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;



// Prototipo de función para obtener la triangulación de un rectángulo
void getRectTriangulation(float b_side, float l_side, int points, int bd_points, CDT* t, std::unordered_map<CDT::Face_handle, 
bool>& in_domain_map);

class RectangleTriangulation {
    private:
        float b_side, l_side;
        int points = 0;

        std::vector<std::vector<Point_2>> voronoi_segments; // Segmentos del diagrama de Voronoi
        std::unordered_map<CDT::Face_handle, bool> in_domain_map; // Mapa de caras en el dominio
        boost::associative_property_map< std::unordered_map<CDT::Face_handle,bool>> in_domain; // Mapa de propiedades asociadas para caras en el dominio

    public:
        CDT rect; // Triangulación de Delaunay con restricciones del rectángulo

        // Constructor de la clase
        RectangleTriangulation(float b_side, float l_side, int points, int bd_points);
        
        // Métodos de la clase
        void compute_voronoi(); // Computa el diagrama de Voronoi
        void draw_delaunay(); // Dibuja la triangulación de Delaunay
        void write_delaunay_off(const std::string& filename); // Escribe la triangulación de Delaunay en un archivo OFF
        void write_voronoi_off(const std::string& filename); // Escribe el diagrama de Voronoi en un archivo OFF
};