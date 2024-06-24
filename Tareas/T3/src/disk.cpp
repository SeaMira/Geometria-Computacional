#include <Disk/disk.hpp>
Segment_2 crop_and_extract_segment(const Segment_2& s, float radius, int points, float pcnt);

bool isInDisk(Point_2 p, float r, int points, float pcnt) {
    int amountBorder = pcnt * (float)points;
    float angleIncrement = 2 * M_PI / (float)amountBorder;

    for (int i = 0; i < amountBorder; i++) {
        float angle1 = i * angleIncrement;
        float angle2 = (i + 1) * angleIncrement;

        float x1 = r * cos(angle1);
        float y1 = r * sin(angle1);

        float x2 = r * cos(angle2);
        float y2 = r * sin(angle2);

        // Convert the segment endpoints to vectors
        float dx1 = x2 - x1;
        float dy1 = y2 - y1;
        
        // Convert the point relative to the segment start point to a vector
        float dx2 = p.x() - x1;
        float dy2 = p.y() - y1;
        
        // Calculate the cross product
        float cross_product = dx1 * dy2 - dy1 * dx2;

        // If the cross product is negative, the point is to the right of the segment
        if (cross_product < 0) {
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
        float angle = i * angleIncrement;
        float x1 = r * cos(angle);
        float y1 = r * sin(angle);

        float x2 = r * cos(angle+angleIncrement);
        float y2 = r * sin(angle+angleIncrement);

        t.insert_constraint(Point_2(x1,y1), Point_2(x2,y2));
    }

    assert(t.is_valid());

    for (int i = 0; i < amountInnerPoints; i++) {
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/r*cos(angleIncrement/2)));
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

        std::cout << "---- " << vit->point().x() << " " << vit->point().y() << std::endl;
        // Use Face_circulator to iterate over incident faces in cyclic order
        CDT::Face_circulator fcirc = disk.incident_faces(vit), done(fcirc);
        bool is_on_border = false;
        if (fcirc != 0) {
            do {
                if (!disk.is_infinite(fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(disk.triangle(fcirc));
                    std::cout << circ_center.x() << " " << circ_center.y() << std::endl;

                    bool has_infinite_neighbour = false;
                    // Check if the face has an infinite neighbor and the circumcenter is inside the disk
                    for (int i = 0; i < 3; ++i) {
                        if (disk.is_infinite(fcirc->neighbor(i))) {
                            if (isInDisk(circ_center, radius, points, pcnt)) {
                                // Calculate the intersection point
                                Point_2 v1 = fcirc->vertex((i + 1) % 3)->point();
                                Point_2 v2 = fcirc->vertex((i + 2) % 3)->point();
                                Point_2 midpoint = CGAL::midpoint(v1, v2);

                                // Determine if points are in counter-clockwise order
                                float determinant = (vit->point().x() * (circ_center.y() - midpoint.y())) +
                                                    (circ_center.x() * (midpoint.y() - vit->point().y())) +
                                                    (midpoint.x() * (vit->point().y() - circ_center.y()));

                                if (determinant > 0) {
                                    circ_centers.push_back(circ_center);
                                    circ_centers.push_back(midpoint);
                                } else {
                                    circ_centers.push_back(midpoint);
                                    circ_centers.push_back(circ_center);
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

        for (size_t i = 0; i < circ_centers.size(); ++i) {
            Point_2 source = circ_centers[i];
            Point_2 target = circ_centers[(i + 1) % circ_centers.size()];

            Segment_2 voronoi_edge(source, target);
            Segment_2 newSeg = crop_and_extract_segment(voronoi_edge, radius, points, pcnt);
            sector_segments.push_back(newSeg);
        }
        voronoi_segments.push_back(sector_segments);
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

    // Crear un mapa para los vértices únicos
    std::map<Point_2, int> vertex_map;
    int index = 0;
    for (const auto& sector : voronoi_segments) {
        for (const auto& seg : sector) {
            if (vertex_map.find(seg.source()) == vertex_map.end()) {
                vertex_map[seg.source()] = index++;
            }
            if (vertex_map.find(seg.target()) == vertex_map.end()) {
                vertex_map[seg.target()] = index++;
            }
        }
    }

    // Escribir el número de vértices y caras
    out << vertex_map.size() << " " << voronoi_segments.size() << " 0\n";

    // Escribir los vértices
    for (const auto& entry : vertex_map) {
        out << entry.first.x() << " " << entry.first.y() << " 0\n";
    }

    // Escribir los sectores como caras, evitando índices repetidos
    for (const auto& sector : voronoi_segments) {
        std::set<int> unique_indices;
        for (const auto& seg : sector) {
            unique_indices.insert(vertex_map[seg.source()]);
            unique_indices.insert(vertex_map[seg.target()]);
        }

        // Escribir la cara con los índices únicos
        out << unique_indices.size();
        for (int idx : unique_indices) {
            out << " " << idx;
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
    int amountBorder = pcnt*(float)points;
    float angleIncrement = 2 * M_PI / (float)amountBorder;

    Point_2 source = s.source();
    Point_2 target = s.target();

    float angle_source = angleFromPoint(source);
    int init = static_cast<int>(angle_source / angleIncrement);
    float distance_source = static_cast<float>(std::sqrt(CGAL::squared_distance(source, Point_2(0, 0))));

    float angle_target = angleFromPoint(target);
    int end = static_cast<int>(angle_target / angleIncrement);
    float distance_target = static_cast<float>(sqrt(CGAL::squared_distance(target, Point_2(0, 0))));
    std::list<Point_2> inters;
    for (int i = init; i < end+1; i++) {
        Point_2 diskp1 = Point_2(cos(angleIncrement*i), sin(angleIncrement*i));
        Point_2 diskp2 = Point_2(cos(angleIncrement*(i+1)), sin(angleIncrement*(i+1)));
        // calcular intersecciones entre segment y el segmento formado por diskp1 y diskp2
        Segment_2 disk_segment(diskp1, diskp2);
        auto result = CGAL::intersection(s, disk_segment);
        if (result) {
            if (const Point_2* p = boost::get<Point_2>(&*result)) {
                inters.push_back(*p);
            } else if (const Segment_2* seg = boost::get<Segment_2>(&*result)) {
                return *seg;
            }
        }
    }

    if (inters.empty()) {
        // Ambos extremos del segmento están dentro o fuera del círculo
        if (distance_source <= radius && distance_target <= radius) {
            // Ambos puntos están dentro del círculo
            return s;
        }
        // Si ambos puntos están fuera, no hacemos nada
    } else if (inters.size() == 1) {
        // Un extremo está dentro y el otro está fuera
        Point_2 intersection_point = inters.front();
        if (distance_source <= radius*cos(angleIncrement/2)) {
            // Source está dentro
            return Segment_2(source, intersection_point);
        } else {
            // Target está dentro
            return Segment_2(target, intersection_point);
        }
    } else if (inters.size() == 2) {
        // Ambos extremos del segmento cruzan el borde del círculo
        auto it = inters.begin();
        Point_2 inter1 = *it++;
        Point_2 inter2 = *it;
        return Segment_2(inter1, inter2);
    }
    return s;
}