#include <Disk/disk.hpp>

Segment_2 crop_and_extract_segment(const Segment_2& s, float radius, int points, float pcnt);
void removeDuplicates(std::vector<Point_2>& myVector);
bool isInDisk(Point_2 p, float r, int points, float pcnt);
bool arePointsIn(std::vector<Point_2>& vec, float radius, int points, float pcnt);
std::vector<Point_2> extractValidSegments(std::vector<Point_2>& pts, float radius, int points, float pcnt);

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
CDT getCircleTriangulation(float r, int pointsA, float pcnt) {
    srand(time(0));
    int amountBorder = pcnt*(float)pointsA;
    int amountInnerPoints = pointsA-amountBorder;

    CDT t;

    float angleIncrement = 2 * M_PI / (float)amountBorder;
    for (int i = 0; i < amountBorder-1; i++) {
        float angle = (float)i * angleIncrement;
        float x1 = r * cos(angle);
        float y1 = r * sin(angle);

        float x2 = r * cos(angle+angleIncrement);
        float y2 = r * sin(angle+angleIncrement);

        t.insert_constraint(Point_2(x1,y1), Point_2(x2,y2));
    }

    assert(t.is_valid());

    for (int i = 0; i < amountInnerPoints; i++) {
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/abs(r*cos(angleIncrement/2) - 0.01)));
        float y_range = sqrt(pow(r*cos(angleIncrement/2), 2) - pow(x, 2)); 
        float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/y_range));
        float x_sign = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
        float y_sign = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;

        x = x_sign > 0.5 ? x : -x;
        y = y_sign > 0.5 ? y : -y;
        t.insert(Point_2(x,y));
    }
    return t;

}

void DiskTriangulation::compute_voronoi() {
    for (auto vit = disk.finite_vertices_begin(); vit != disk.finite_vertices_end(); ++vit) {
        std::vector<Point_2> circ_centers;
        std::vector<Segment_2> sector_segments;

        // std::cout << "------------ " << vit->point().x() << " " << vit->point().y() << std::endl;
        // Use Face_circulator to iterate over incident faces in cyclic order
        CDT::Face_circulator fcirc = disk.incident_faces(vit), done(fcirc);
        bool is_on_border = false;
        if (fcirc != 0) {
            do {
                if (!disk.is_infinite(fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(disk.triangle(fcirc));
                    
                    bool has_infinite_neighbour = false;
                    // Check if the face has an infinite neighbor and the circumcenter is inside the disk
                    for (int i = 0; i < 3; ++i) {
                        Point_2 v1 = fcirc->vertex((i + 1) % 3)->point();
                        Point_2 v2 = fcirc->vertex((i + 2) % 3)->point();
                        if (disk.is_infinite(fcirc->neighbor(i)) && (v1 == vit->point() || v2 == vit->point())) {
                            if (isInDisk(circ_center, radius, points, pcnt)) {
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

        std::vector<Point_2> cropped_segments = extractValidSegments(circ_centers, radius, points, pcnt);
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



DiskTriangulation::DiskTriangulation(float radius, int points, float pcnt) : radius(radius), points(points), pcnt(pcnt) {
    disk = getCircleTriangulation(radius, points, pcnt);
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