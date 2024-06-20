#include <Rectangle/rectangle.hpp>


bool is_point_in_L(const Point_2& p, float b_side, float l_side) {
    if (p.x() < 0 || p.x() > b_side || p.y() < 0 || p.y() > b_side) {
        return false;
    }
    if (p.x() > b_side - l_side && p.y() > b_side - l_side) {
        return false;
    }
    return true;
}

void mark_domain(CDT& t, std::unordered_map<CDT::Face_handle, bool>& in_domain_map, float b_side, float l_side) {
    for (auto fit = t.all_faces_begin(); fit != t.all_faces_end(); ++fit) {
        if (t.is_infinite(fit)) {
            in_domain_map[fit] = false;
        } else {
            Point_2 p = CGAL::centroid(t.triangle(fit));
            in_domain_map[fit] = is_point_in_L(p, b_side, l_side);
        }
    }
}


// Creates a disk of radius r with a pointsA amount of points. A percentage (pcnt) of them are on the border and the rest are randomly inside.
void getRectTriangulation(float b_side, float l_side, int points, int bd_points, CDT* t, std::unordered_map<CDT::Face_handle, bool>& in_domain_map) {
    srand(time(0));

    int l_bd_points = (float)bd_points * (l_side/b_side);
    int b_bd_points = bd_points-l_bd_points;

    // CDT t;
    float dt = b_side / (float)bd_points;
    float b_dt = (b_side-l_side) / (float)b_bd_points;
    float l_dt = l_side / (float)l_bd_points;

    std::vector<Point_2> pointsL;
    for (int i = 0; i < bd_points+1; i++) pointsL.push_back(Point_2(dt*(float)i,0.0f));
    for (int i = 0; i < b_bd_points+1; i++) pointsL.push_back(Point_2(b_side,b_dt*(float)i));
    for (int i = 0; i < l_bd_points+1; i++) pointsL.push_back(Point_2(b_side-l_dt*(float)i,b_side-l_side));
    for (int i = 0; i < l_bd_points+1; i++) pointsL.push_back(Point_2(b_side-l_side,b_side-l_side+l_dt*(float)i));
    for (int i = 0; i < b_bd_points+1; i++) pointsL.push_back(Point_2(b_side-l_side-b_dt*(float)i,b_side));
    for (int i = 0; i < bd_points+1; i++) pointsL.push_back(Point_2(0.0f,dt*(float)i));
    
    for (size_t i = 0; i < pointsL.size(); ++i) {
        t->insert_constraint(pointsL[i], pointsL[(i + 1) % pointsL.size()]);
    }

    // std::cout << "alo" << std::endl;

    assert(t->is_valid());

    for (int i = 0; i < points; i++) {
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/b_side));
        float y;
        if (x > b_side-l_side) y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(b_side-l_side))); 
        else y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/b_side));
        t->insert(Point_2(x,y));
    }

    mark_domain(*t, in_domain_map, b_side, l_side);
}

RectangleTriangulation::RectangleTriangulation(float b_side, float l_side, int points, int bd_points) : b_side(b_side), l_side(l_side), points(points), in_domain(in_domain_map) {
    if (l_side > b_side) l_side = b_side*0.5f;
    getRectTriangulation(b_side, l_side, points, bd_points, &rect, in_domain_map);
    in_domain = boost::associative_property_map<std::unordered_map<CDT::Face_handle, bool>>(in_domain_map);
    // compute_voronoi();
}

// void RectangleTriangulation::compute_voronoi() {
//     Cropped_voronoi_from_delaunay voronoi_cropped;
//     for (auto fit = rect.finite_faces_begin(); fit != rect.finite_faces_end(); ++fit) {
//         Point_2 circ_center = CGAL::circumcenter(rect.triangle(fit));
//         for (int i = 0; i < 3; ++i) {
//             CDT::Face_handle neighbor = fit->neighbor(i);
//             if (rect.is_infinite(neighbor)) continue;
//             Point_2 neighbor_circ_center = CGAL::circumcenter(rect.triangle(neighbor));
//             Segment_2 voronoi_edge(circ_center, neighbor_circ_center);
//             voronoi_cropped.crop_and_extract_segment(voronoi_edge, radius, points, pcnt);
//         }
//     }
//     cropped_vd_disk = voronoi_cropped.m_cropped_vd;
// }


void RectangleTriangulation::draw_delaunay() {
    if (rect.is_valid()) {
        // Iterar sobre todas las aristas y dibujar solo aquellas cuyas dos caras adyacentes están dentro del dominio
        for (auto eit = rect.finite_edges_begin(); eit != rect.finite_edges_end(); ++eit) {
            CDT::Face_handle fh = eit->first;
            int index = eit->second;
            CDT::Face_handle opposite_fh = fh->neighbor(index);

            if (get(in_domain, fh) && get(in_domain, opposite_fh)) {
                auto source = fh->vertex((index + 1) % 3)->point();
                auto target = fh->vertex((index + 2) % 3)->point();
            }
        }
        CGAL::draw(rect, in_domain);
    }
}


void RectangleTriangulation::write_delaunay_off(const std::string& filename) {
    std::ofstream out(filename);
    out << "OFF\n";

    // Escribir los vértices
    std::map<Point_2, int> vertex_map;
    int index = 0;

    int faces = 0;
    for (auto fit = rect.finite_faces_begin(); fit != rect.finite_faces_end(); ++fit) {
        if (!get(in_domain, fit)) continue;
        faces++;
    }

    out << rect.number_of_vertices() << " " << faces << " 0\n";
    for (auto vit = rect.finite_vertices_begin(); vit != rect.finite_vertices_end(); ++vit) {
        Point_2 p = vit->point();
        out << p.x() << " " << p.y() << " 0\n";
        vertex_map[p] = index++;
    }

    // Escribir las caras
    for (auto fit = rect.finite_faces_begin(); fit != rect.finite_faces_end(); ++fit) {
        if (!get(in_domain, fit)) continue;
        out << "3 ";
        for (int i = 0; i < 3; ++i) {
            Point_2 p = fit->vertex(i)->point();
            out << vertex_map[p] << " ";
        }
        out << "\n";
    }

    out.close();
}

// void RectangleTriangulation::write_voronoi_off(const std::string& filename) {
//     std::ofstream out(filename);
//     out << "OFF\n";

//     // Crear un mapa para los vértices únicos
//     std::map<Point_2, int> vertex_map;
//     int index = 0;
//     for (const auto& seg : cropped_vd_disk) {
//         if (vertex_map.find(seg.source()) == vertex_map.end()) {
//             vertex_map[seg.source()] = index++;
//         }
//         if (vertex_map.find(seg.target()) == vertex_map.end()) {
//             vertex_map[seg.target()] = index++;
//         }
//     }

//     // Escribir el número de vértices y aristas
//     out << vertex_map.size() << " 0 " << cropped_vd_disk.size()/2 << "\n";

//     // Escribir los vértices
//     for (const auto& entry : vertex_map) {
//         out << entry.first.x() << " " << entry.first.y() << " 0\n";
//     }

//     // Usar un set para almacenar aristas únicas
//     std::set<std::pair<int, int>> edges;
//     for (const auto& seg : cropped_vd_disk) {
//         int v1 = vertex_map[seg.source()];
//         int v2 = vertex_map[seg.target()];
//         if (v1 > v2) std::swap(v1, v2); // Asegurar que el menor índice esté primero
//         edges.insert(std::make_pair(v1, v2));
//     }

//     // Escribir las aristas únicas
//     for (const auto& edge : edges) {
//         out << "2 " << edge.first << " " << edge.second << "\n";
//     }

//     out.close();
// }


