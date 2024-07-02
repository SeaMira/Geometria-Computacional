#include <Disk/disk.hpp>

Segment_2 crop_and_extract_segment(const Segment_2& s, float radius, int points, float pcnt);
void removeDuplicates(std::vector<Point_2>& myVector);
bool isInDisk(Point_2 p, float r, int points, float pcnt);
bool arePointsIn(std::vector<Point_2>& vec, float radius, int points, float pcnt);
std::vector<Point_2> extractValidSegments(std::vector<Point_2>& pts, float radius, int points, float pcnt);


Point_2 getCircumcenter(CDT::Face_handle face) {
    return CGAL::circumcenter(face->vertex(0)->point(), face->vertex(1)->point(), face->vertex(2)->point());
}

std::vector<Point_2> getSharedPoints(CDT::Face_handle f, CDT::Face_handle f_next) {
    std::vector<Point_2> shared_points;

    // Itera sobre los vértices de la cara f
    for (int i = 0; i < 3; ++i) {
        CDT::Vertex_handle v = f->vertex(i);

        // Verifica si este vértice está en f_next
        for (int j = 0; j < 3; ++j) {
            if (v == f_next->vertex(j)) {
                shared_points.push_back(v->point());
                break;
            }
        }
    }

    return shared_points;
}

Point_2 getMidpoint(const std::vector<Point_2>& points) {
    if (points.size() != 2) {
        throw std::runtime_error("Expected exactly two points to calculate the midpoint.");
    }
    return CGAL::midpoint(points[0], points[1]);
}

int orientation(Point_2 p1, Point_2 p2, Point_2 p3)
{
    // See 10th slides from following link for derivation
    // of the formula
    float val = (p2.y() - p1.y()) * (p3.x() - p2.x())
              - (p2.x() - p1.x()) * (p3.y() - p2.y());
 
    if (val == 0.0f)
        return 0; // collinear
 
    return (val > 0.0f) ? 1 : 2; // clock or counterclock wise
}

bool isInDisk(Point_2 p, float r, int points, float pcnt) {
    int amountBorder = pcnt * (float)points;
    float angleIncrement = 2 * M_PI / (float)amountBorder;
    if (p.x()*p.x()+p.y()*p.y() > r*r) return false;
    for (int i = 0; i < amountBorder; i++) {
        float angle1 = (float)i * angleIncrement;
        float angle2 = (float)(i + 1) * angleIncrement;

        float x1 = r * cos(angle1);
        float y1 = r * sin(angle1);

        float x2 = r * cos(angle2);
        float y2 = r * sin(angle2);

        // If the cross product is positive, the point is to the right of the segment
        if (orientation(Point_2(x1,y1), Point_2(x2,y2), p) == 1) {
            return false;
        }
    }
    return true;
}

// Creates a disk of radius r with a pointsA amount of points. A percentage (pcnt) of them are on the border and the rest are randomly inside.
void DiskTriangulation::getCircleTriangulation(CDT& t, float r, int pointsA, float pcnt) {
    int amountBorder = pcnt*(float)pointsA;
    int amountInnerPoints = pointsA-amountBorder;


    float angleIncrement = 2 * M_PI / (float)amountBorder;
    for (int i = 0; i < amountBorder-1; i++) {
        float angle = (float)i * angleIncrement;
        float angleInc = (float)(i+1) * angleIncrement;
        float x1 = r * cos(angle);
        float y1 = r * sin(angle);

        float x2 = r * cos(angleInc);
        float y2 = r * sin(angleInc);
        bd_points.push_back(Point_2(x1,y1));
        if (i == amountBorder-2) bd_points.push_back(Point_2(x2,y2));

        t.insert_constraint(Point_2(x1,y1), Point_2(x2,y2));
    }

    assert(t.is_valid());

    std::default_random_engine gen;
    gen.seed(std::time(0));
    std::uniform_real_distribution<float> rad(0.0, r*cos(angleIncrement/2.0f)*0.95f);
    std::uniform_real_distribution<float> theta(0.0, 2 * M_PI);
    for (int i = 0; i < amountInnerPoints; i++) {
        
        float R = rad(gen);
        float angle = theta(gen);
        float x = R * cos(angle);
        float y = R * sin(angle);
        t.insert(Point_2(x,y));
    }

}

int findInd(Point_2 const & p, std::vector<Point_2>& v) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == p) return i;
    }
    return -1;
}

// void tagBlindTriangles(CDT& t, std::unordered_map<CDT::Face_handle, std::pair<bool, Segment_2>>& is_blind) {

//     // Initialize all faces as non-blind
//     for (auto fit = t.all_faces_begin(); fit != t.all_faces_end(); ++fit) {
//         is_blind[fit] = std::make_pair(false, Segment_2(Point_2(0.0f, 0.0f),Point_2(0.0f, 0.0f)));
//     }

//     // Iterate over each constrained edge
//     for (auto eit = t.constrained_edges_begin(); eit != t.constrained_edges_end(); ++eit) {
//         std::stack<CDT::Face_handle> triangles;

//         CDT::Face_handle f1 = eit->first;
//         int i1 = eit->second;

//         Point_2 p1 = f1->vertex(f1->cw(i1))->point();
//         Point_2 p2 = f1->vertex(f1->ccw(i1))->point();

//         if (!is_blind[f1].first && !t.is_infinite(f1)) {
//             triangles.push(f1);
//             while (!triangles.empty()) {
//                 CDT::Face_handle f = triangles.top();
//                 triangles.pop();

//                 if (orientation(p1, p2, getCircumcenter(f)) == 1) {
//         std::cout << "p1 " << p1.x() << "," << p1.y() << " p2 " << p2.x() << "," << p2.y() << std::endl;
//                     // Tag f as blind
//                     is_blind[f].first = true;
//                     is_blind[f].second = Segment_2(p1, p2);

//                     // Push non-blind adjacent triangles with unconstrained common edge to the stack
//                     for (int i = 0; i < 3; ++i) {
//                         CDT::Face_handle f_adj = f->neighbor(i);
//                         int adj_index = f->index(f_adj);
//                         if (!t.is_infinite(f_adj) && !is_blind[f_adj].first && !t.is_constrained(std::make_pair(f, i))) {
//                             triangles.push(f_adj);
//                         }
//                     }
//                 }
//             }
//         }

//         CDT::Face_handle f2 = f1->neighbor(i1);

//         // Push both adjacent triangles to the stack if they are non-blind
//         if (!is_blind[f2].first && !t.is_infinite(f2)) {
//             triangles.push(f2);
        
//             triangles.push(f1);
//             while (!triangles.empty()) {
//                 CDT::Face_handle f = triangles.top();
//                 triangles.pop();

//                 if (orientation(p1, p2, getCircumcenter(f)) == 1) {
//                     // Tag f as blind
//                     is_blind[f].first = true;
//                     is_blind[f].second = Segment_2(p1, p2);

//                     // Push non-blind adjacent triangles with unconstrained common edge to the stack
//                     for (int i = 0; i < 3; ++i) {
//                         CDT::Face_handle f_adj = f->neighbor(i);
//                         int adj_index = f->index(f_adj);
//                         if (!t.is_infinite(f_adj) && !is_blind[f_adj].first && !t.is_constrained(std::make_pair(f, i))) {
//                             triangles.push(f_adj);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

void DiskTriangulation::compute_voronoi() {
    for (auto vit = disk.finite_vertices_begin(); vit != disk.finite_vertices_end(); ++vit) {
        std::vector<Point_2> circ_centers;

        // std::cout << "------------ " << vit->point().x() << " " << vit->point().y() << std::endl;
        // Use Face_circulator to iterate over incident faces in cyclic order
        CDT::Face_circulator fcirc = disk.incident_faces(vit), done(fcirc);
        if (fcirc != 0) {
            do {
                if (!disk.is_infinite(fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(disk.triangle(fcirc));
                    
                    // bool has_infinite_neighbour = false;
                    // // Check if the face has an infinite neighbor and the circumcenter is inside the disk
                    // for (int i = 0; i < 3; ++i) {
                    //     Point_2 v1 = fcirc->vertex((i + 1) % 3)->point();
                    //     Point_2 v2 = fcirc->vertex((i + 2) % 3)->point();
                    //     if (disk.is_infinite(fcirc->neighbor(i))  && (v1 == vit->point() || v2 == vit->point())) {
                    //         // Calculate the intersection point
                    //         Point_2 midpoint = CGAL::midpoint(v1, v2);
                    //         if (isInDisk(circ_center, radius, points, pcnt)) {

                    //             // Determine if points are in counter-clockwise order
                    //             int orient = orientation(vit->point(), midpoint, circ_center);

                    //             if (orient == 2) { // 2 indicates counter-clockwise
                    //                 circ_centers.push_back(midpoint);
                    //                 if (std::find(circ_centers.begin(), circ_centers.end(), circ_center) == circ_centers.end()) circ_centers.push_back(circ_center);
                    //             } else {
                    //                 if (std::find(circ_centers.begin(), circ_centers.end(), circ_center) == circ_centers.end()) circ_centers.push_back(circ_center);
                    //                 circ_centers.push_back(midpoint);
                    //             }
                    //         } else {
                    //             circ_centers.push_back(midpoint);
                    //         }
                    //         has_infinite_neighbour = true;
                    //     }
                    // }
                    circ_centers.push_back(circ_center);
                } else {
                    int thisPind = findInd(Point_2(vit->point()), bd_points);
                    int prevPind = thisPind == 0? bd_points.size()-1 :thisPind-1;
                    int nextPind = (thisPind+1)%bd_points.size();

                    Point_2 prevMidP = CGAL::midpoint(bd_points[thisPind], bd_points[prevPind]);
                    Point_2 nextMidP = CGAL::midpoint(bd_points[thisPind], bd_points[nextPind]);

                    circ_centers.push_back(prevMidP);
                    circ_centers.push_back(Point_2(vit->point()));
                    circ_centers.push_back(nextMidP);
                }
            } while (++fcirc != done);
        }
        
        removeDuplicates(circ_centers);

        // Eliminate duplicate points while maintaining order
        // std::vector<Point_2> unique_circ_centers;
        // std::set<Point_2> seen;
        // for (const auto& point : circ_centers) {
        //     if (seen.insert(point).second) {
        //         unique_circ_centers.push_back(point);
        //     }
        // }

        // std::vector<Point_2> cropped_segments = extractValidSegments(circ_centers, radius, points, pcnt);
        // while (!arePointsIn(cropped_segments, radius, points, pcnt)) {
        //     cropped_segments = extractValidSegments(cropped_segments, radius, points, pcnt);
        // }
        // removeDuplicates(cropped_segments);
        // for (int i = 0; i < circ_centers.size(); i++) {
        //     std::cout << circ_centers[i].x() << " " << circ_centers[i].y() << std::endl;
        // }
        // for (int i = 0; i < cropped_segments.size(); i++) {
        //     std::cout << cropped_segments[i].x() << " " << cropped_segments[i].y() << std::endl;
        // }
        // std::cout << circ_centers.size() << std::endl;
        voronoi_segments.push_back(circ_centers);

        
    }
}

// std::vector<Point_2> DiskTriangulation::extractValidSegments(std::vector<CDT::Face_handle>& pts) {
//     std::vector<Point_2> circ_centers;
//     std::vector<Point_2> vertx;
//     for (size_t i = 0; i < pts.size(); ++i) {
//         if (!disk.is_infinite(pts[i])) {
//             Point_2 circ_center = CGAL::circumcenter(disk.triangle(pts[i]));
//             circ_centers.push_back(circ_center);
//         } else {
//             circ_centers.push_back(Point_2(radius, radius));
//         }
//     }

//     for (size_t i = 0; i < pts.size(); ++i) {
//         if (!disk.is_infinite(pts[i])) {

//             if (!in_domain_map[pts[i]].first) {
//                 vertx.push_back(circ_centers[i]);

//                 if (disk.is_infinite(pts[(i+1) % pts.size()])) {
//                     for (int j = 0; j<3; j++) {
//                         CDT::Vertex_handle v1 = pts[i]->vertex((j + 1) % 3);
//                         CDT::Vertex_handle v2 = pts[i]->vertex((j + 2) % 3);
//                         if (disk.is_infinite(v1) && disk.is_infinite(v2)) {
//                             Point_2 midpoint = CGAL::midpoint(v1->point(), v2->point());
//                             vertx.push_back(midpoint);
//                         }
//                     }
//                 } else if (in_domain_map[pts[(i+1) % pts.size()]].first) {
//                     Segment_2 s_des(circ_centers[i], circ_centers[(i+1) % pts.size()]);
//                     Segment_2 S_cons = in_domain_map[pts[(i+1) % pts.size()]].second;
//                     auto intersection = CGAL::intersection(s_des, S_cons);
//                     if (const Point_2* p3 = boost::get<Point_2>(&*intersection)) {
//                         vertx.push_back(*p3);
//                     }
//                 }
//             } else {
//                 if (!in_domain_map[pts[(i+1) % pts.size()]].first) {
//                     Segment_2 s_des(circ_centers[i], circ_centers[(i+1) % pts.size()]);
//                     Segment_2 S_cons = in_domain_map[pts[i]].second;
//                     auto intersection = CGAL::intersection(s_des, S_cons);
//                     if (const Point_2* p3 = boost::get<Point_2>(&*intersection)) {
//                         vertx.push_back(*p3);
//                     }
//                 }
//             }

//         } else {
//             if (!in_domain_map[pts[(i+1) % pts.size()]].first) {
//                 for (int j = 0; j<3; j++) {
//                     CDT::Vertex_handle v1 = pts[(i+1) % pts.size()]->vertex((j + 1) % 3);
//                     CDT::Vertex_handle v2 = pts[(i+1) % pts.size()]->vertex((j + 2) % 3);
//                     if (disk.is_infinite(v1) && disk.is_infinite(v2)) {
//                         Point_2 midpoint = CGAL::midpoint(v1->point(), v2->point());
//                         vertx.push_back(midpoint);
//                     }
//                 }
//             }
//         }
//     }

//     return vertx;
// }


// void DiskTriangulation::compute_voronoi() {
//     for (auto vit = disk.finite_vertices_begin(); vit != disk.finite_vertices_end(); ++vit) {
//         std::vector<CDT::Point_2> circ_centers;
//         std::vector<Point_2> vertx;

//         CDT::Face_circulator fcirc = disk.incident_faces(vit), done(fcirc);
//         if (fcirc != 0) {
//             do {
//                 if (!disk.is_infinite(fcirc)) {
//                     Point_2 circ_center = CGAL::circumcenter(disk.triangle(fcirc));
//                     circ_centers.push_back(circ_center);
//                 } else {
//                     circ_centers.push_back(Point_2(radius, radius));
//                 }
//             } while (++fcirc != done);
//         }

//         int size = circ_centers.size();
//         CDT::Face_circulator f_init = disk.incident_faces(vit), done(f_init);
//         CDT::Face_circulator f_c = disk.incident_faces(vit), done(f_c);
//         CDT::Face_circulator f_next = disk.incident_faces(vit), done(f_c);

//         for (size_t i = 0; i < size; ++i) {
//             if (!disk.is_infinite(f_c)) {

//                 if (!in_domain_map[f_c].first) {
//                     vertx.push_back(circ_centers[i]);

//                     if (disk.is_infinite(pts[(i+1) % pts.size()])) {
//                         for (int j = 0; j<3; j++) {
//                             CDT::Vertex_handle v1 = f_c->vertex((j + 1) % 3);
//                             CDT::Vertex_handle v2 = f_c->vertex((j + 2) % 3);
//                             if (disk.is_infinite(v1) && disk.is_infinite(v2)) {
//                                 Point_2 midpoint = CGAL::midpoint(v1->point(), v2->point());
//                                 vertx.push_back(midpoint);
//                             }
//                         }
//                     } else if (in_domain_map[pts[(i+1) % pts.size()]].first) {
//                         Segment_2 s_des(circ_centers[i], circ_centers[(i+1) % pts.size()]);
//                         Segment_2 S_cons = in_domain_map[pts[(i+1) % pts.size()]].second;
//                         auto intersection = CGAL::intersection(s_des, S_cons);
//                         if (const Point_2* p3 = boost::get<Point_2>(&*intersection)) {
//                             vertx.push_back(*p3);
//                         }
//                     }
//                 } else {
//                     if (!in_domain_map[pts[(i+1) % pts.size()]].first) {
//                         Segment_2 s_des(circ_centers[i], circ_centers[(i+1) % pts.size()]);
//                         Segment_2 S_cons = in_domain_map[f_c].second;
//                         auto intersection = CGAL::intersection(s_des, S_cons);
//                         if (const Point_2* p3 = boost::get<Point_2>(&*intersection)) {
//                             vertx.push_back(*p3);
//                         }
//                     }
//                 }

//             } else {
//                 if (!in_domain_map[pts[(i+1) % pts.size()]].first) {
//                     for (int j = 0; j<3; j++) {
//                         CDT::Vertex_handle v1 = pts[(i+1) % pts.size()]->vertex((j + 1) % 3);
//                         CDT::Vertex_handle v2 = pts[(i+1) % pts.size()]->vertex((j + 2) % 3);
//                         if (disk.is_infinite(v1) && disk.is_infinite(v2)) {
//                             Point_2 midpoint = CGAL::midpoint(v1->point(), v2->point());
//                             vertx.push_back(midpoint);
//                         }
//                     }
//                 }
//             }
//         }



      
//         std::vector<Point_2> cropped_segments = this->extractValidSegments(circ_centers);
                
//         voronoi_segments.push_back(cropped_segments);

//     }
// }


DiskTriangulation::DiskTriangulation(float radius, int points, float pcnt) : radius(radius), points(points), pcnt(pcnt), in_domain(in_domain_map) {
    getCircleTriangulation(disk, radius, points, pcnt);
    // tagBlindTriangles(disk, in_domain_map);
    // in_domain = boost::associative_property_map<std::unordered_map<CDT::Face_handle, std::pair<bool, Segment_2>>>(in_domain_map);
    compute_voronoi();
}

void DiskTriangulation::draw_delaunay() {
    if (disk.is_valid()) {
        std::unordered_map<CDT::Face_handle, bool> in_domain_map;
        boost::associative_property_map< std::unordered_map<CDT::Face_handle,bool>> in_domain(in_domain_map);
        CGAL::draw(disk, in_domain);
    }
}


void DiskTriangulation::write_delaunay_off(const std::string& filename) {
    std::ofstream out(filename);
    out << "OFF\n";

    // Escribir los vértices
    std::map<Point_2, int> vertex_map;
    int index = 0;
    out << disk.number_of_vertices() << " " << disk.number_of_faces() << " 0\n";
    for (auto vit = disk.finite_vertices_begin(); vit != disk.finite_vertices_end(); ++vit) {
        Point_2 p = vit->point();
        out << p.x() << " " << p.y() << " 0\n";
        vertex_map[p] = index++;
    }

    // Escribir las caras
    for (auto fit = disk.finite_faces_begin(); fit != disk.finite_faces_end(); ++fit) {
        out << "3 ";
        for (int i = 0; i < 3; ++i) {
            Point_2 p = fit->vertex(i)->point();
            out << vertex_map[p] << " ";
        }
        out << "\n";
    }

    out.close();
}

void DiskTriangulation::write_voronoi_off(const std::string& filename) {
    std::ofstream out(filename);
    out << "OFF\n";

    // // Crear un mapa para los vértices únicos
    // std::map<Point_2, int> vertex_map;
    // int index = 0;
    // for (const auto& sector : voronoi_segments) {
    //     for (const auto& seg : sector) {
    //         if (vertex_map.find(seg.source()) == vertex_map.end()) {
    //             vertex_map[seg.source()] = index++;
    //         }
    //         if (vertex_map.find(seg.target()) == vertex_map.end()) {
    //             vertex_map[seg.target()] = index++;
    //         }
    //     }
    // }

    // // Escribir el número de vértices y caras
    // out << vertex_map.size() << " " << voronoi_segments.size() << " 0\n";

    // // Escribir los vértices
    // for (const auto& entry : vertex_map) {
    //     out << entry.first.x() << " " << entry.first.y() << " 0\n";
    // }

    // // Escribir los sectores como caras, evitando índices repetidos
    // for (const auto& sector : voronoi_segments) {
    //     std::set<int> unique_indices;
    //     for (const auto& seg : sector) {
    //         unique_indices.insert(vertex_map[seg.source()]);
    //         unique_indices.insert(vertex_map[seg.target()]);
    //     }

    //     // Escribir la cara con los índices únicos
    //     out << unique_indices.size();
    //     for (int idx : unique_indices) {
    //         out << " " << idx;
    //     }
    //     out << "\n";
    // }

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



float angleFromPoint(Point_2 p) {
    float angle;
    if (p.x() == 0.0f) {
        return p.y() > 0.0f ? M_PI/2.0f : 3.0f*M_PI/2.0f;
    }
    angle = std::atan2(static_cast<float>(p.y()), static_cast<float>(p.x())); 
    return angle > 0.0f ? angle : angle + 2.0f*M_PI;
    
}

Segment_2 crop_and_extract_segment(const Segment_2& s, float radius, int points, float pcnt) {
    int amountBorder = pcnt * (float)points;
    float angleIncrement = 2 * M_PI / (float)amountBorder;

    Point_2 source = s.source();
    Point_2 target = s.target();

    std::vector<Point_2> inters;

    // Function to find the intersection of a segment with a border segment
    auto intersect_with_segment = [](const Segment_2& s1, const Segment_2& s2) -> std::vector<Point_2> {
        std::vector<Point_2> result;
        auto res = CGAL::intersection(s1, s2);
        if (res) {
            if (const Point_2* ip = boost::get<Point_2>(&*res)) {
                result.push_back(*ip);
            } else if (const Segment_2* sp = boost::get<Segment_2>(&*res)) {
                result.push_back(sp->source());
                result.push_back(sp->target());
            }
        }
        return result;
    };

    // Iterate over the border segments
    for (int i = 0; i < amountBorder; ++i) {
        float angle1 = (float)i * angleIncrement;
        float angle2 = (float)(i + 1) * angleIncrement;
        Point_2 p1 = Point_2(radius * cos(angle1), radius * sin(angle1));
        Point_2 p2 = Point_2(radius * cos(angle2), radius * sin(angle2));
        Segment_2 border_segment(p1, p2);

        auto intersection_points = intersect_with_segment(s, border_segment);
        inters.insert(inters.end(), intersection_points.begin(), intersection_points.end());
    }

    if (inters.empty()) {
        // Ambos extremos del segmento están dentro o fuera del disco
        if (isInDisk(source, radius, points, pcnt) && isInDisk(target, radius, points, pcnt)) return s;
        else {
            std::cout << "Source están afuera " << source.x() << " " << source.y() << " tar " << target.x() << " " << target.y() << std::endl;
            return Segment_2(Point_2(1, 1), Point_2(1, 1));
        }
    } else if (inters.size() == 1) {
        // Un extremo está dentro y el otro está fuera
        Point_2 intersection_point = inters.front();
        if (isInDisk(source, radius, points, pcnt)) {
            // Source está dentro
            std::cout << "Source está dentro " << source.x() << " " << source.y() << " inter " << intersection_point.x() << " " << intersection_point.y() << std::endl;
            return Segment_2(source, intersection_point);
        } else if (isInDisk(target, radius, points, pcnt)) {
            // Target está dentro
            std::cout << "Target está dentro " << target.x() << " " << target.y() << " inter " << intersection_point.x() << " " << intersection_point.y() << std::endl;
            return Segment_2(intersection_point, target);
        } else return Segment_2(intersection_point, intersection_point);
    } else if (inters.size() >= 2) {
        // Ambos extremos del segmento cruzan el borde del disco
        std::cout << "ambos cruzan borde " << inters.front().x() << " " << inters.front().y() << " inter " << inters.back().x() << " " << inters.back().y() << std::endl;
        return Segment_2(inters.front(), inters.back());
    }
    return Segment_2(Point_2(0, 0), Point_2(0, 0)); // Return an invalid segment by default
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

bool arePointsIn(std::vector<Point_2>& vec, float radius, int points, float pcnt) {
    for (int i = 0; i< vec.size(); i++) {
        if (!isInDisk(vec[i], radius, points, pcnt)) return false;
    }
    return true;
}

std::vector<Point_2> extractValidSegments(std::vector<Point_2>& pts, float radius, int points, float pcnt) {
    std::vector<Point_2> cropped_segments;
        for (size_t i = 0; i < pts.size(); ++i) {
            Point_2 source = pts[i];
            Point_2 target = pts[(i + 1) % pts.size()];

            Segment_2 voronoi_edge(source, target);
            Segment_2 newSeg = crop_and_extract_segment(voronoi_edge, radius, points, pcnt);
            if (newSeg.source() != Point_2(0, 0) || newSeg.target() != Point_2(0, 0)) { // Ensure valid segment
                if (newSeg.source() == Point_2(1, 1) && newSeg.target() == Point_2(1, 1)) {
                    continue;
                }
                cropped_segments.push_back(newSeg.source());
                if (newSeg.source() != newSeg.target()) cropped_segments.push_back(newSeg.target());
            }
        }
    return cropped_segments;
}