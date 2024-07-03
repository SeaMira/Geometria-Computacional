#include <Pikachu/pikachu.hpp>

std::vector<Point_2> extractValidSegments(std::vector<Point_2>& vor_cell, Point_2 voronoi_sector);

int orientation(Point_2 p1, Point_2 p2, Point_2 p3) {
    // See 10th slides from following link for derivation
    // of the formula
    float val = (p2.y() - p1.y()) * (p3.x() - p2.x())
              - (p2.x() - p1.x()) * (p3.y() - p2.y());
 
    if (val == 0.0f)
        return 0; // collinear
 
    return (val > 0.0f) ? 1 : 2; // clock or counterclock wise
}

void removeDuplicates(std::vector<Point_2>& myVector) { 
    std::unordered_set<Point_2> seen; 
  
    // Using remove_if to eliminate duplicates and get the 
    // new end iterator 
    auto newEnd = remove_if( 
        myVector.begin(), myVector.end(), 
        [&seen](Point_2& value) { 
            // Checking if value has been seen; if not, add 
            // to seen and keep in vector 
            if (seen.find(value) == seen.end()) { 
                seen.insert(value); 
                return false; // Don't remove the item 
            } 
            return true; // Remove the item 
        }); 
  
    // Erase the non-unique elements 
    myVector.erase(newEnd, myVector.end()); 
} 

bool is_point_in_triangle(const Point_2& p, const Point_2& a, const Point_2& b, const Point_2& c) {
    return (CGAL::orientation(a, b, p) != CGAL::RIGHT_TURN &&
            CGAL::orientation(b, c, p) != CGAL::RIGHT_TURN &&
            CGAL::orientation(c, a, p) != CGAL::RIGHT_TURN);
}

PikachuTriangulation::PikachuTriangulation(const std::string& filename): in_domain(in_domain_map) {
    std::ifstream input(filename);
    if (!input) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(input, line);
    if (line != "OFF") {
        std::cerr << "Formato incorrecto: se esperaba OFF al inicio del archivo" << std::endl;
        return;
    }

    int num_vertices, num_faces, num_edges;
    input >> num_vertices >> num_faces >> num_edges;

    std::vector<Point_2> vertices(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        double x, y, z;
        input >> x >> y >> z;
        vertices[i] = Point_2(x, y);
    }

    std::vector<CDT::Vertex_handle> vertex_handles(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        vertex_handles[i] = pikachu.insert(vertices[i]);
    }

    for (int i = 0; i < num_faces; ++i) {
        int num_face_vertices;
        input >> num_face_vertices;
        if (num_face_vertices != 3) {
            std::cerr << "Se esperaba una cara triangular" << std::endl;
            return;
        }

        int idx1, idx2, idx3;
        input >> idx1 >> idx2 >> idx3;
        pikachu.insert_constraint(vertex_handles[idx1], vertex_handles[idx2]);
        pikachu.insert_constraint(vertex_handles[idx2], vertex_handles[idx3]);
        pikachu.insert_constraint(vertex_handles[idx3], vertex_handles[idx1]);
    }

    for (auto fit = pikachu.all_faces_begin(); fit != pikachu.all_faces_end(); ++fit) {
        if (pikachu.is_infinite(fit)) {
            in_domain_map[fit] = false;
        } else {
            bool all_edges_constrained = true;
            for (int i = 0; i < 3; ++i) {
                if (!pikachu.is_constrained(CDT::Edge(fit, i))) {
                    all_edges_constrained = false;
                    break;
                }
            }
            in_domain_map[fit] = all_edges_constrained;
        }
    }

    // Recorrer todas las caras para encontrar los segmentos del borde
    for (auto fit = pikachu.finite_faces_begin(); fit != pikachu.finite_faces_end(); ++fit) {
        for (int i = 0; i < 3; ++i) {
            CDT::Face_handle neighbor = fit->neighbor(i);
            if (pikachu.is_infinite(neighbor) || !in_domain_map[neighbor]) {
                // Obtener los puntos del segmento
                Point_2 v1 = fit->vertex((i + 1) % 3)->point();
                Point_2 v2 = fit->vertex((i + 2) % 3)->point();
                // Agregar el segmento al vector border
                border.push_back(Segment_2(v1, v2));
            }
        }
    }

    compute_voronoi();
}


void PikachuTriangulation::compute_voronoi() {
    for (auto vit = pikachu.finite_vertices_begin(); vit != pikachu.finite_vertices_end(); ++vit) {
        std::vector<Point_2> circ_centers;
        std::vector<VCP> vor_cell;

        // std::cout << "------------ " << vit->point().x() << " " << vit->point().y() << std::endl;
        // Use Face_circulator to iterate over incident faces in cyclic order
        CDT::Face_circulator fcirc = pikachu.incident_faces(vit), done(fcirc);
        if (fcirc != 0) {
            do {
                if (!pikachu.is_infinite(fcirc) && get(in_domain, fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(pikachu.triangle(fcirc));
                    
                    bool has_infinite_neighbour = false;
                    // Check if the face has an infinite neighbor and the circumcenter is inside the pikachu
                    for (int i = 0; i < 3; ++i) {
                        Point_2 v1 = fcirc->vertex((i + 1) % 3)->point();
                        Point_2 v2 = fcirc->vertex((i + 2) % 3)->point();
                        if ((pikachu.is_infinite(fcirc->neighbor(i)) || !get(in_domain, fcirc->neighbor(i))) && (v1 == vit->point() || v2 == vit->point())) {
                            bool circ_center_in_domain = false;
                            for (auto fit = pikachu.finite_faces_begin(); fit != pikachu.finite_faces_end(); ++fit) {
                                if (in_domain_map[fit]) {
                                    Point_2 v0 = fit->vertex(0)->point();
                                    Point_2 v1 = fit->vertex(1)->point();
                                    Point_2 v2 = fit->vertex(2)->point();
                                    if (is_point_in_triangle(circ_center, v0, v1, v2)) {
                                        circ_center_in_domain = true;
                                        break;
                                    }
                                }
                            }
                            if (circ_center_in_domain) {
                                // Calculate the intersection point
                                Point_2 midpoint = CGAL::midpoint(v1, v2);

                                // Determine if points are in counter-clockwise order
                                int orient = orientation(vit->point(), midpoint, circ_center);

                                if (orient == 2) { // 2 indicates counter-clockwise
                                    circ_centers.push_back(midpoint);
                                    if (std::find(circ_centers.begin(), circ_centers.end(), circ_center) == circ_centers.end()) circ_centers.push_back(circ_center);
                                } else {
                                    if (std::find(circ_centers.begin(), circ_centers.end(), circ_center) == circ_centers.end()) circ_centers.push_back(circ_center);
                                    circ_centers.push_back(midpoint);
                                }
                                has_infinite_neighbour = true;
                            } 
                        }
                    }
                    if (!has_infinite_neighbour) circ_centers.push_back(circ_center);
                } else {
                    if (std::find(circ_centers.begin(), circ_centers.end(), Point_2(vit->point())) == circ_centers.end()) circ_centers.push_back(Point_2(vit->point()));
                }
            } while (++fcirc != done);
        }
        
        removeDuplicates(circ_centers);
        circ_centers = extractValidSegments(circ_centers, Point_2(vit->point()));
        removeDuplicates(circ_centers);
        voronoi_segments.push_back(circ_centers);
    }
}

void PikachuTriangulation::draw_delaunay() {
    if (pikachu.is_valid()) {
        CGAL::draw(pikachu, in_domain);
    }
}


void PikachuTriangulation::write_delaunay_off(const std::string& filename) {
    std::ofstream out(filename);
    out << "OFF\n";

    // Escribir los vértices
    std::map<Point_2, int> vertex_map;
    int index = 0;

    int faces = 0;
    for (auto fit = pikachu.finite_faces_begin(); fit != pikachu.finite_faces_end(); ++fit) {
        if (!get(in_domain, fit)) continue;
        faces++;
    }

    out << pikachu.number_of_vertices() << " " << faces << " 0\n";
    for (auto vit = pikachu.finite_vertices_begin(); vit != pikachu.finite_vertices_end(); ++vit) {
        Point_2 p = vit->point();
        out << p.x() << " " << p.y() << " 0\n";
        vertex_map[p] = index++;
    }

    // Escribir las caras
    for (auto fit = pikachu.finite_faces_begin(); fit != pikachu.finite_faces_end(); ++fit) {
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

void PikachuTriangulation::write_voronoi_off(const std::string& filename) {
    std::ofstream out(filename);
    out << "OFF\n";

    int index = 0;
    int vertex_amount = 0;
    for (const auto& sector : voronoi_segments) {
        for (const auto& vert : sector) {
            vertex_amount++;
        }
    }

    out << vertex_amount << " " << voronoi_segments.size() << " 0\n";


    for (const auto& sector : voronoi_segments) {
        for (const auto& vert : sector) {
            out << vert.x() << " " << vert.y() << " 0\n";
        }
    }

    for (const auto& sector : voronoi_segments) {
        out << sector.size();
        for (const auto& vert : sector) {
            out << " " << index++;
        }
        out << "\n";
    }
    

    out.close();
}


double calculate_angle(const Point_2& p, Point_2& center) {
    return atan2(p.y() - center.y(), p.x() - center.x());
}


std::vector<Point_2> PikachuTriangulation::extractValidSegments(std::vector<Point_2>& vor_cell, Point_2 voronoi_sector) {
    std::vector<Point_2> cropped_segments;
        for (size_t i = 0; i < vor_cell.size(); ++i) {
            Point_2 src = vor_cell[i];
            Point_2 tgt = vor_cell[(i + 1) % vor_cell.size()];

            Segment_2 s(src, tgt);

            bool src_in = false, tgt_in = false;
            for (auto fit = pikachu.finite_faces_begin(); fit != pikachu.finite_faces_end(); ++fit) {
                if (in_domain_map[fit]) {
                    Point_2 v0 = fit->vertex(0)->point();
                    Point_2 v1 = fit->vertex(1)->point();
                    Point_2 v2 = fit->vertex(2)->point();
                    if (is_point_in_triangle(src, v0, v1, v2)) {
                        src_in = true;
                        break;
                    }
                }
            }
            for (auto fit = pikachu.finite_faces_begin(); fit != pikachu.finite_faces_end(); ++fit) {
                if (in_domain_map[fit]) {
                    Point_2 v0 = fit->vertex(0)->point();
                    Point_2 v1 = fit->vertex(1)->point();
                    Point_2 v2 = fit->vertex(2)->point();
                    if (is_point_in_triangle(tgt, v0, v1, v2)) {
                        tgt_in = true;
                        break;
                    }
                }
            }

            if (src_in && !tgt_in) {
                cropped_segments.push_back(src);
                std::cout << " src dentro target fuera " << std::endl; 
                for (const auto& border_segment : border) {
                    auto intersection = CGAL::intersection(s, border_segment);
                    if (intersection) {
                        if (const Point_2* p = boost::get<Point_2>(&*intersection)) {
                            cropped_segments.push_back(*p);
                            break;
                        }
                    }
                }

            } else if (!src_in && tgt_in) {
                std::cout << " src fuera target dentro " << std::endl; 
                for (const auto& border_segment : border) {
                    auto intersection = CGAL::intersection(s, border_segment);
                    if (intersection) {
                        if (const Point_2* p = boost::get<Point_2>(&*intersection)) {
                            cropped_segments.push_back(*p);
                            break;
                        }
                    }
                }

            } else if (src_in && tgt_in) {
                std::cout << " src dentro target dentro " << std::endl; 
                cropped_segments.push_back(src);
            }
            removeDuplicates(cropped_segments);

            std::cout << src.x()  << " " << src.y() << std::endl; 
            std::cout << tgt.x()  << " " << tgt.y() << std::endl; 
        }

    //     // Reordenar los puntos en cropped_segments en sentido antihorario alrededor de voronoi_sector
    // std::sort(cropped_segments.begin(), cropped_segments.end(),
    //           [&voronoi_sector](const Point_2& a, const Point_2& b) {
    //               return calculate_angle(a, voronoi_sector) < calculate_angle(b, voronoi_sector);
    //           });
    //     std::cout << cropped_segments.size() << std::endl; 
    return cropped_segments;
}