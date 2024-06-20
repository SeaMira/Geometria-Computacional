#include <Disk/disk.hpp>

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
    Cropped_voronoi_from_delaunay voronoi_cropped;
    for (auto fit = disk.finite_faces_begin(); fit != disk.finite_faces_end(); ++fit) {
        Point_2 circ_center = CGAL::circumcenter(disk.triangle(fit));
        for (int i = 0; i < 3; ++i) {
            CDT::Face_handle neighbor = fit->neighbor(i);
            if (disk.is_infinite(neighbor)) continue;
            Point_2 neighbor_circ_center = CGAL::circumcenter(disk.triangle(neighbor));
            Segment_2 voronoi_edge(circ_center, neighbor_circ_center);
            voronoi_cropped.crop_and_extract_segment(voronoi_edge, radius, points, pcnt);
        }
    }
    cropped_vd_disk = voronoi_cropped.m_cropped_vd;
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
    for (const auto& seg : cropped_vd_disk) {
        if (vertex_map.find(seg.source()) == vertex_map.end()) {
            vertex_map[seg.source()] = index++;
        }
        if (vertex_map.find(seg.target()) == vertex_map.end()) {
            vertex_map[seg.target()] = index++;
        }
    }

    // Escribir el número de vértices y aristas
    out << vertex_map.size() << " " << cropped_vd_disk.size()/2 << " 0\n";

    // Escribir los vértices
    for (const auto& entry : vertex_map) {
        out << entry.first.x() << " " << entry.first.y() << " 0\n";
    }

    // Usar un set para almacenar aristas únicas
    std::set<std::pair<int, int>> edges;
    for (const auto& seg : cropped_vd_disk) {
        int v1 = vertex_map[seg.source()];
        int v2 = vertex_map[seg.target()];
        if (v1 > v2) std::swap(v1, v2); // Asegurar que el menor índice esté primero
        edges.insert(std::make_pair(v1, v2));
    }

    // Escribir las aristas únicas
    for (const auto& edge : edges) {
        out << "2 " << edge.first << " " << edge.second << "\n";
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

void Cropped_voronoi_from_delaunay::crop_and_extract_segment(const Segment_2& s, float radius, int points, float pcnt) {
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
                m_cropped_vd.push_back(*seg);
                return ;
            }
        }
    }

    if (inters.empty()) {
        // Ambos extremos del segmento están dentro o fuera del círculo
        if (distance_source <= radius && distance_target <= radius) {
            // Ambos puntos están dentro del círculo
            m_cropped_vd.push_back(s);
        }
        // Si ambos puntos están fuera, no hacemos nada
    } else if (inters.size() == 1) {
        // Un extremo está dentro y el otro está fuera
        Point_2 intersection_point = inters.front();
        if (distance_source <= radius*cos(angleIncrement/2)) {
            // Source está dentro
            m_cropped_vd.push_back(Segment_2(source, intersection_point));
        } else {
            // Target está dentro
            m_cropped_vd.push_back(Segment_2(target, intersection_point));
        }
    } else if (inters.size() == 2) {
        // Ambos extremos del segmento cruzan el borde del círculo
        auto it = inters.begin();
        Point_2 inter1 = *it++;
        Point_2 inter2 = *it;
        m_cropped_vd.push_back(Segment_2(inter1, inter2));
    }
}



