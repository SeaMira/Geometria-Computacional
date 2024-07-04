#include <Disk/disk.hpp>

void removeDuplicates(std::vector<Point_2>& myVector);


// getCircleTriangulation: Crea una triangulación de Delaunay para un disco de radio r
// con pointsA puntos, de los cuales un porcentaje pcnt se encuentra en el borde y 
// el resto dentro del disco.
// t: La triangulación de Delaunay.
// r: Radio del disco.
// pointsA: Número total de puntos a insertar.
// pcnt: Porcentaje de puntos que están en el borde.
void DiskTriangulation::getCircleTriangulation(CDT& t, float r, int pointsA, float pcnt) {
    // puntos en borde
    int amountBorder = pcnt*(float)pointsA;
    // puntos en interior
    int amountInnerPoints = pointsA-amountBorder;

    // angulo de incrementos para ingresar borde
    float angleIncrement = 2 * M_PI / (float)amountBorder;
    for (int i = 0; i < amountBorder-1; i++) {
        float angle = (float)i * angleIncrement;
        float angleInc = (float)(i+1) * angleIncrement;
        float x1 = r * cos(angle);
        float y1 = r * sin(angle);

        float x2 = r * cos(angleInc);
        float y2 = r * sin(angleInc);
        // se guarda además los puntos del borde en sentido ccw
        bd_points.push_back(Point_2(x1,y1));
        if (i == amountBorder-2) bd_points.push_back(Point_2(x2,y2));
        // se colocan como restricción
        t.insert_constraint(Point_2(x1,y1), Point_2(x2,y2));
    }
    // verifica validez de borde
    assert(t.is_valid());

    // se colocan puntos aleatorios dentro del disco 
    std::default_random_engine gen;
    gen.seed(std::time(0));
    // se hace en un rango menor al radio para asegurar triángulos interiores con circuncentro
    // dentro del dominio
    std::uniform_real_distribution<float> rad(0.0, r*cos(angleIncrement/2.0f)*0.9f);
    std::uniform_real_distribution<float> theta(0.0, 2 * M_PI);
    for (int i = 0; i < amountInnerPoints; i++) {
        
        float R = rad(gen);
        float angle = theta(gen);
        float x = R * cos(angle);
        float y = R * sin(angle);
        t.insert(Point_2(x,y));
    }

}

// funcion para encontrar el índice de un punto en un vector de puntos. Si no está, retorna -1
int findInd(Point_2 const & p, std::vector<Point_2>& v) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == p) return i;
    }
    return -1;
}


// Computa el diagrama de Voronoi para la triangulación del rectángulo guardando 
// vectores con puntos que representan una celda de voronoi. Se guardan en ccw.
void DiskTriangulation::compute_voronoi() {
    // se itera sobre todos los vérices de la triangulación
    // pues son centros de sectores de voronoi
    for (auto vit = disk.finite_vertices_begin(); vit != disk.finite_vertices_end(); ++vit) {
        std::vector<Point_2> circ_centers;

        // Se circula a través de las caras adyacentes al vértice
        CDT::Face_circulator fcirc = disk.incident_faces(vit), done(fcirc);
        if (fcirc != 0) {
            do {
                // Si la cara actual no es infinita se procede agregando el circuncentro simplemente
                if (!disk.is_infinite(fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(disk.triangle(fcirc));
                    circ_centers.push_back(circ_center);
                // si es infinita se agregan los puntos medios de los bordes adyacentes al vertice revisado además del mismo vértice
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
        
        // por seguridad se remueven duplicados
        removeDuplicates(circ_centers);

        // se agrega la celda de voronoi
        voronoi_segments.push_back(circ_centers);

        
    }
}


// DiskTriangulation: Constructor que inicializa la triangulación del disco y computa su diagrama de Voronoi.
// radius: Radio del disco.
// points: Número de puntos a insertar.
// pcnt: Porcentaje de puntos que están en el borde.
DiskTriangulation::DiskTriangulation(float radius, int points, float pcnt) : radius(radius), points(points), pcnt(pcnt) {
    getCircleTriangulation(disk, radius, points, pcnt);
    compute_voronoi();
}

// dibuja la triangulación con cgal, para debug solamente
void DiskTriangulation::draw_delaunay() {
    if (disk.is_valid()) {
        std::unordered_map<CDT::Face_handle, bool> in_domain_map;
        boost::associative_property_map< std::unordered_map<CDT::Face_handle,bool>> in_domain(in_domain_map);
        CGAL::draw(disk, in_domain);
    }
}

//  Dibuja la triangulación de Delaunay en un archivo OFF
// filename: Nombre del archivo donde se guardará la triangulación.
void DiskTriangulation::write_delaunay_off(const std::string& filename) {
    // abre archivo
    std::ofstream out(filename);
    // primera linea
    out << "OFF\n";

    // Escribir los vértices una única vez
    std::map<Point_2, int> vertex_map;
    int index = 0;
    
    // se escribe la 2da linea de off: vertices, caras y segmentos
    out << disk.number_of_vertices() << " " << disk.number_of_faces() << " 0\n";
    // a cada vertice le asigna un índice en el mapa
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
    // cierra archivo
    out.close();
}

// Escribe el diagrama de Voronoi en un archivo OFF
// Parámetros:
// - filename: Nombre del archivo donde se guardará el diagrama de Voronoi
void DiskTriangulation::write_voronoi_off(const std::string& filename) {
    // abre archivo
    std::ofstream out(filename);
    // primera linea
    out << "OFF\n";
    
    // en este caso se decidió escribir vértices repetidos en el off pues se tiene un
    // vector de vectores de puntos representando las celdas, con los puntos ya en orden
    int index = 0;
    int vertex_amount = 0;
    // se cuentan vertices
    for (const auto& sector : voronoi_segments) {
        for (const auto& vert : sector) {
            vertex_amount++;
        }
    }
    // se escriben cantidad de vertices y caras
    out << vertex_amount << " " << voronoi_segments.size() << " 0\n";

    // se escriben los vertices
    for (const auto& sector : voronoi_segments) {
        for (const auto& vert : sector) {
            out << vert.x() << " " << vert.y() << " 0\n";
        }
    }
    // se escriben las caras
    for (const auto& sector : voronoi_segments) {
        out << sector.size();
        for (const auto& vert : sector) {
            out << " " << index++;
        }
        out << "\n";
    }
    

    out.close();
}

void DiskTriangulation::write_delaunay_node_ele(const std::string& node_filename, const std::string& ele_filename) {
    // Escribir el archivo .node
    std::ofstream node_out(node_filename);
    node_out << disk.number_of_vertices() << " 2 0 0\n"; // número de puntos, dimensiones, atributos, marcas

    std::map<Point_2, int> vertex_map;
    int index = 0;
    for (auto vit = disk.finite_vertices_begin(); vit != disk.finite_vertices_end(); ++vit) {
        Point_2 p = vit->point();
        node_out << index << " " << p.x() << " " << p.y() << "\n";
        vertex_map[p] = index++;
    }
    node_out.close();

    // Escribir el archivo .ele
    std::ofstream ele_out(ele_filename);
    ele_out << disk.number_of_faces() << " 3 0\n"; // número de elementos, nodos por elemento, atributos

    index = 0;
    for (auto fit = disk.finite_faces_begin(); fit != disk.finite_faces_end(); ++fit) {
        ele_out << index << " ";
        for (int i = 0; i < 3; ++i) {
            Point_2 p = fit->vertex(i)->point();
            ele_out << vertex_map[p] << " ";
        }
        ele_out << "\n";
        ++index;
    }
    ele_out.close();
}

void DiskTriangulation::write_voronoi_node_ele(const std::string& node_filename, const std::string& ele_filename) {
    // Escribir el archivo .node
    std::ofstream node_out(node_filename);

    int vertex_count = 0;
    for (const auto& sector : voronoi_segments) {
        vertex_count += sector.size();
    }
    node_out << vertex_count << " 2 0 0\n"; // número de puntos, dimensiones, atributos, marcas

    int index = 0;
    std::map<Point_2, int> vertex_map;
    for (const auto& sector : voronoi_segments) {
        for (const auto& vert : sector) {
            node_out << index << " " << vert.x() << " " << vert.y() << "\n";
            vertex_map[vert] = index++;
        }
    }
    node_out.close();

    // Escribir el archivo .ele
    std::ofstream ele_out(ele_filename);
    ele_out << voronoi_segments.size() << " " << 3 << " 0\n"; // número de elementos, nodos por elemento, atributos

    index = 0;
    for (const auto& sector : voronoi_segments) {
        ele_out << index << " ";
        for (const auto& vert : sector) {
            ele_out << vertex_map[vert] << " ";
        }
        ele_out << "\n";
        ++index;
    }
    ele_out.close();
}


// Elimina duplicados de un vector de puntos
// Parámetros:
// - myVector: Vector de puntos del cual se eliminarán duplicados
void removeDuplicates(std::vector<Point_2>& myVector) { 
    std::unordered_set<Point_2> seen; 
  
    // Se remueve si el punto revisado ya está en seen
    auto newEnd = remove_if( 
        myVector.begin(), myVector.end(), 
        [&seen](Point_2& value) { 
            // Checkea si está en seen o no 
            if (seen.find(value) == seen.end()) { 
                seen.insert(value); 
                return false; // No se remueve si se llega al final de seen sin verlo
            } 
            return true; // Se remueve en caso contrario
        }); 
  
    // Se aplica la función definida para la condición de eliminación en newEnd 
    myVector.erase(newEnd, myVector.end()); 
} 

