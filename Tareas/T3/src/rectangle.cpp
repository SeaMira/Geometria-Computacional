#include <Rectangle/rectangle.hpp>

Segment_2 crop_and_extract_segment(const Segment_2& s, float b_side, float l_side);
std::vector<Point_2> extractValidSegments(std::vector<Point_2>& pts, float b_side, float l_side);

bool is_point_in_L(const Point_2& p, float b_side, float l_side) {
    if (p.x() < 0 || p.x() > b_side || p.y() < 0 || p.y() > b_side) {
        return false;
    }
    if (p.x() > b_side - l_side && p.y() > b_side - l_side) {
        return false;
    }
    return true;
}


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
    compute_voronoi();
}

void RectangleTriangulation::compute_voronoi() {
    for (auto vit = rect.finite_vertices_begin(); vit != rect.finite_vertices_end(); ++vit) {
        std::vector<Point_2> circ_centers;
        std::vector<Segment_2> sector_segments;

        // std::cout << "------------ " << vit->point().x() << " " << vit->point().y() << std::endl;
        // Use Face_circulator to iterate over incident faces in cyclic order
        CDT::Face_circulator fcirc = rect.incident_faces(vit), done(fcirc);
        bool is_on_border = false;
        if (fcirc != 0) {
            do {
                if (!rect.is_infinite(fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(rect.triangle(fcirc));
                    
                    bool has_infinite_neighbour = false;
                    // Check if the face has an infinite neighbor and the circumcenter is inside the rect
                    for (int i = 0; i < 3; ++i) {
                        Point_2 v1 = fcirc->vertex((i + 1) % 3)->point();
                        Point_2 v2 = fcirc->vertex((i + 2) % 3)->point();
                        if (rect.is_infinite(fcirc->neighbor(i)) && (v1 == vit->point() || v2 == vit->point())) {
                            if (is_point_in_L(circ_center, b_side, l_side)) {
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
                    is_on_border = true;
                    circ_centers.push_back(Point_2(vit->point()));
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

        std::vector<Point_2> cropped_segments = extractValidSegments(circ_centers, b_side, l_side);
        // while (!arePointsIn(cropped_segments, radius, points, pcnt)) {
        //     cropped_segments = extractValidSegments(cropped_segments, radius, points, pcnt);
        // }
        removeDuplicates(cropped_segments);
        // for (int i = 0; i < circ_centers.size(); i++) {
        //     std::cout << circ_centers[i].x() << " " << circ_centers[i].y() << std::endl;
        // }
        // for (int i = 0; i < cropped_segments.size(); i++) {
        //     std::cout << cropped_segments[i].x() << " " << cropped_segments[i].y() << std::endl;
        // }
        // std::cout << circ_centers.size() << std::endl;
        voronoi_segments.push_back(cropped_segments);
    }
}


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

void RectangleTriangulation::write_voronoi_off(const std::string& filename) {
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

std::vector<Point_2> inter_border(Segment_2 const& s, float b_side, float l_side) {
    Segment_2 b_b(Point_2(0.0f, 0.0f), Point_2(b_side, 0.0f));
    Segment_2 b_r(Point_2(b_side, 0.0f), Point_2(b_side, b_side-l_side));
    Segment_2 l_b(Point_2(b_side, b_side-l_side), Point_2(b_side-l_side, b_side-l_side));
    Segment_2 l_l(Point_2(b_side-l_side, b_side-l_side), Point_2(b_side-l_side, b_side));
    Segment_2 b_t(Point_2(b_side-l_side, b_side), Point_2(0.0f, b_side));
    Segment_2 b_l(Point_2(0.0f, b_side), Point_2(0.0f, 0.0f));
    std::vector<Segment_2> segs = {b_b, b_r, l_b, l_l, b_t, b_l};

    std::vector<Point_2> result;
    for (int i = 0; i < segs.size(); i++) {
        auto res = CGAL::intersection(s, segs[i]);
        if (res) {
            if (const Point_2* ip = boost::get<Point_2>(&*res)) {
                result.push_back(*ip);
            } else if (const Segment_2* sp = boost::get<Segment_2>(&*res)) {
                result.push_back(sp->source());
                result.push_back(sp->target());
            }
        }
    }
    return result;

}


Segment_2 crop_and_extract_segment(const Segment_2& s, float b_side, float l_side) {
    Point_2 src = s.source();
    Point_2 tgt = s.target();

    if (is_point_in_L(src, b_side, l_side) && is_point_in_L(tgt, b_side, l_side)) {
        return s;
    } else if (is_point_in_L(src, b_side, l_side) && !is_point_in_L(tgt, b_side, l_side)) {
        std::vector<Point_2> inters =inter_border(s, b_side, l_side);
        if (inters.size() == 1) {
            Point_2 intersection_point = inters.front();
            return Segment_2(src, intersection_point);
        } else return Segment_2(src, src);
    } else if (!is_point_in_L(src, b_side, l_side) && is_point_in_L(tgt, b_side, l_side)) {
        std::vector<Point_2> inters =inter_border(s, b_side, l_side);
        if (inters.size() == 1) {
            Point_2 intersection_point = inters.front();
            return Segment_2(intersection_point, tgt);
        } else return Segment_2(tgt, tgt);
    } else return Segment_2(Point_2(0, 0), Point_2(0, 0));

}

std::vector<Point_2> extractValidSegments(std::vector<Point_2>& pts, float b_side, float l_side) {
    std::vector<Point_2> cropped_segments;
        for (size_t i = 0; i < pts.size(); ++i) {
            Point_2 source = pts[i];
            Point_2 target = pts[(i + 1) % pts.size()];

            Segment_2 voronoi_edge(source, target);
            Segment_2 newSeg = crop_and_extract_segment(voronoi_edge, b_side, l_side);
            if (newSeg.source() != Point_2(0, 0) || newSeg.target() != Point_2(0, 0)) { // Ensure valid segment
                cropped_segments.push_back(newSeg.source());
                if (newSeg.source() != newSeg.target()) cropped_segments.push_back(newSeg.target());
            }
        }
    return cropped_segments;
}