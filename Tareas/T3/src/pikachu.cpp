#include <Pikachu/pikachu.hpp>


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


