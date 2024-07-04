#include <Rectangle/rectangle.hpp>

Segment_2 crop_and_extract_segment(const Segment_2& s, float b_side, float l_side);
std::vector<Point_2> extractValidSegments(std::vector<Point_2>& pts, float b_side, float l_side);


// Verifica si un punto está dentro de una región en forma de L en el rectángulo
// Parámetros:
// - p: Punto a verificar
// - b_side: Tamaño del lado del cuadrado mayor
// - l_side: Tamaño del lado del cuadrado menor extraído
bool is_point_in_L(const Point_2& p, float b_side, float l_side) {
    // si está fuera del cuadrado mayor
    if (p.x() < 0 || p.x() > b_side || p.y() < 0 || p.y() > b_side) {
        return false;
    }
    // si está dentro del cuadrado menor
    if (p.x() > b_side - l_side && p.y() > b_side - l_side) {
        return false;
    }
    // si está dentro de la L
    return true;
}

// Determina la orientación de tres puntos
// Parámetros:
// - p1: Primer punto
// - p2: Segundo punto
// - p3: Tercer punto
// visto en clases
int orientation(Point_2 p1, Point_2 p2, Point_2 p3) {
    float val = (p2.y() - p1.y()) * (p3.x() - p2.x())
              - (p2.x() - p1.x()) * (p3.y() - p2.y());
 
    if (val == 0.0f)
        return 0; // colineal
 
    return (val > 0.0f) ? 1 : 2; // cw o ccw
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

// Marca las caras del dominio en la triangulación
// Parámetros:
// - t: Triangulación de Delaunay con restricciones
// - in_domain_map: Mapa para almacenar las caras del dominio
// - b_side: Tamaño del lado del cuadrado mayor
// - l_side: Tamaño del lado del cuadrado menor extraído
void mark_domain(CDT& t, std::unordered_map<CDT::Face_handle, bool>& in_domain_map, float b_side, float l_side) {
    for (auto fit = t.all_faces_begin(); fit != t.all_faces_end(); ++fit) {
        // si la cara es infinita no pertenece al dominio
        if (t.is_infinite(fit)) {
            in_domain_map[fit] = false;
        // se verifica si el centroide del triángulo está en la L (el centroide es un punto promedio a los tres vertices)
        } else {
            Point_2 p = CGAL::centroid(t.triangle(fit));
            in_domain_map[fit] = is_point_in_L(p, b_side, l_side);
        }
    }
}


// Crea una triangulación de un rectángulo, insertando puntos y bordes
// Parámetros:
// - b_side: Tamaño del lado del cuadrado mayor
// - l_side: Tamaño del lado del cuadrado menor extraído
// - points: Número de puntos a insertar
// - bd_points: Número de puntos en el borde
// - t: Puntero a la triangulación de Delaunay con restricciones
// - in_domain_map: Mapa para almacenar las caras del dominio
void getRectTriangulation(float b_side, float l_side, int points, int bd_points, CDT* t, std::unordered_map<CDT::Face_handle, bool>& in_domain_map) {
    srand(time(0));

    // puntos de bordes "no cortados"
    int l_bd_points = (float)bd_points * (l_side/b_side);
    // puntos de bordes "cortados"
    int b_bd_points = bd_points-l_bd_points;

    // delta de distancia entre puntos en bordes no cortados
    float dt = b_side / (float)bd_points;
    // delta de distancia entre puntos en bordes cortados (parte grande)
    float b_dt = (b_side-l_side) / (float)b_bd_points;
    // delta de distancia entre puntos en bordes cortados (parte extraída)
    float l_dt = l_side / (float)l_bd_points;

    // se colocan los puntos del borde
    std::vector<Point_2> pointsL;
    for (int i = 0; i < bd_points+1; i++) pointsL.push_back(Point_2(dt*(float)i,0.0f));
    for (int i = 0; i < b_bd_points+1; i++) pointsL.push_back(Point_2(b_side,b_dt*(float)i));
    for (int i = 0; i < l_bd_points+1; i++) pointsL.push_back(Point_2(b_side-l_dt*(float)i,b_side-l_side));
    for (int i = 0; i < l_bd_points+1; i++) pointsL.push_back(Point_2(b_side-l_side,b_side-l_side+l_dt*(float)i));
    for (int i = 0; i < b_bd_points+1; i++) pointsL.push_back(Point_2(b_side-l_side-b_dt*(float)i,b_side));
    for (int i = 0; i < bd_points+1; i++) pointsL.push_back(Point_2(0.0f,dt*(float)i));
    
    // se asignan los puntos como restricción
    for (size_t i = 0; i < pointsL.size(); ++i) {
        t->insert_constraint(pointsL[i], pointsL[(i + 1) % pointsL.size()]);
    }
    // checkeo de validez de la restricción
    assert(t->is_valid());

    // se insertan puntos dentro de la L teniendo cuidado de que no sea en el cuadrado extraído
    for (int i = 0; i < points; i++) {
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/b_side));
        float y;
        if (x > b_side-l_side) y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(b_side-l_side))); 
        else y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/b_side));
        t->insert(Point_2(x,y));
    }

    // se verifican marcan los triángulos en el dominio
    mark_domain(*t, in_domain_map, b_side, l_side);
}

// Constructor de la clase RectangleTriangulation
// Parámetros:
// - b_side: Tamaño del lado del cuadrado mayor
// - l_side: Tamaño del lado del cuadrado menor extraído
// - points: Número de puntos a insertar
// - bd_points: Número de puntos en el borde
RectangleTriangulation::RectangleTriangulation(float b_side, float l_side, int points, int bd_points) : b_side(b_side), l_side(l_side), points(points), in_domain(in_domain_map) {
    // si se entrega un lado pequeño de tamaño mayor al lado grande, entonces se deja el lado pequeño como la mitad del lado grande
    if (l_side > b_side) l_side = b_side*0.5f;
    // se crea la triangulación
    getRectTriangulation(b_side, l_side, points, bd_points, &rect, in_domain_map);
    // se asigna el dominio
    in_domain = boost::associative_property_map<std::unordered_map<CDT::Face_handle, bool>>(in_domain_map);
    // se calcula voronoi restringido
    compute_voronoi();
}

// Computa el diagrama de Voronoi para la triangulación del rectángulo guardando 
// vectores con puntos que representan una celda de voronoi. Se guardan en ccw.
void RectangleTriangulation::compute_voronoi() {
    // se itera sobre todos los vérices de la triangulación
    // pues son centros de sectores de voronoi
    for (auto vit = rect.finite_vertices_begin(); vit != rect.finite_vertices_end(); ++vit) {
        // vector que contendrá los puntos de la celda en ccw alrededor del punto del sector
        std::vector<Point_2> circ_centers;

        // Se circula a través de las caras adyacentes al vértice
        CDT::Face_circulator fcirc = rect.incident_faces(vit), done(fcirc);
        if (fcirc != 0) {
            do {
                // Si la cara actual no es infinita se procede
                if (!rect.is_infinite(fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(rect.triangle(fcirc));
                    
                    // checkeo si es un borde infinito o no, en caso que se tengan que agregar puntos medios y/o no 
                    // se agregue el circuncentro de la cara revisada más de una vez  
                    bool has_infinite_neighbour = false;
                    // se checkean los vecinos de la cara revisada
                    for (int i = 0; i < 3; ++i) {
                        // puntos compartidos entre la cara revisada y el vecino revisado
                        Point_2 v1 = fcirc->vertex((i + 1) % 3)->point();
                        Point_2 v2 = fcirc->vertex((i + 2) % 3)->point();
                        // si el vecino es infinito o no está en el dominio (estamos en el borde), y alguno de los vértices es el que estamos circulando
                        if ((rect.is_infinite(fcirc->neighbor(i)) || !get(in_domain, fcirc->neighbor(i))) && (v1 == vit->point() || v2 == vit->point())) {
                            // Si el circuncentro de la cara revisada está dentro del dominio entonces se une el punto medio del borde
                            if (is_point_in_L(circ_center, b_side, l_side)) {
                                // Calculo de pto medio
                                Point_2 midpoint = CGAL::midpoint(v1, v2);

                                // Determina qué debe ir primero: punto medio o circuncentro, dependiendo del borde infinito que sea
                                int orient = orientation(vit->point(), midpoint, circ_center);

                                if (orient == 2) { // 2 indicates counter-clockwise
                                    if (std::find(circ_centers.begin(), circ_centers.end(), Point_2(vit->point())) == circ_centers.end()) circ_centers.push_back(Point_2(vit->point()));
                                    circ_centers.push_back(midpoint);
                                    if (std::find(circ_centers.begin(), circ_centers.end(), circ_center) == circ_centers.end()) circ_centers.push_back(circ_center);
                                } else {
                                    if (std::find(circ_centers.begin(), circ_centers.end(), circ_center) == circ_centers.end()) circ_centers.push_back(circ_center);
                                    circ_centers.push_back(midpoint);
                                    if (std::find(circ_centers.begin(), circ_centers.end(), Point_2(vit->point())) == circ_centers.end()) circ_centers.push_back(Point_2(vit->point()));
                                }
                                // se marca como que sí era borde infinito
                                has_infinite_neighbour = true;
                            }
                        }
                    }
                    // si no tenía vecino infinito entonces es triángulo interior o el circuncentro está fuera del dominio
                    if (!has_infinite_neighbour) circ_centers.push_back(circ_center);
                // Si la cara actual es infinita estamos circulando un punto de borde y por lo mismo este se agrega si no se ha agregado ya 
                } else {
                    if (std::find(circ_centers.begin(), circ_centers.end(), Point_2(vit->point())) == circ_centers.end()) circ_centers.push_back(Point_2(vit->point()));
                }
            } while (++fcirc != done);
        }

        // se remueven puntos duplicados
        removeDuplicates(circ_centers);
        // se extraen los puntos validos al calcular intersecciones con el dominio
        std::vector<Point_2> cropped_segments = extractValidSegments(circ_centers, b_side, l_side);
        // se remueven puntos duplicados nuevamente por seguridad
        removeDuplicates(cropped_segments);
        // se agrega la celda 
        voronoi_segments.push_back(cropped_segments);
    }
}

// Dibuja la triangulación de Delaunay
void RectangleTriangulation::draw_delaunay() {
    // si se tiene una triangulación valida se dibuja on la librería de cgal, para debug simplemente
    if (rect.is_valid()) {
        CGAL::draw(rect, in_domain);
    }
}

// Escribe la triangulación de Delaunay en un archivo OFF
// Parámetros:
// - filename: Nombre del archivo donde se guardará la triangulación
void RectangleTriangulation::write_delaunay_off(const std::string& filename) {
    // abre archivo
    std::ofstream out(filename);
    // primera linea
    out << "OFF\n";

    // Escribir los vértices una única vez
    std::map<Point_2, int> vertex_map;
    int index = 0;

    // conteo de caras válidas
    int faces = 0;
    for (auto fit = rect.finite_faces_begin(); fit != rect.finite_faces_end(); ++fit) {
        if (!get(in_domain, fit)) continue;
        faces++;
    }

    // se escribe la 2da linea de off: vertices, caras y segmentos
    out << rect.number_of_vertices() << " " << faces << " 0\n";
    // a cada vertice le asigna un índice en el mapa
    for (auto vit = rect.finite_vertices_begin(); vit != rect.finite_vertices_end(); ++vit) {
        Point_2 p = vit->point();
        out << p.x() << " " << p.y() << " 0\n";
        vertex_map[p] = index++;
    }

    // Escribir las caras de acuerdo al índice del mapa
    for (auto fit = rect.finite_faces_begin(); fit != rect.finite_faces_end(); ++fit) {
        if (!get(in_domain, fit)) continue;
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
void RectangleTriangulation::write_voronoi_off(const std::string& filename) {
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

// Encuentra las intersecciones de un segmento con los bordes del rectángulo
// Parámetros:
// - s: Segmento cuyo cruce con los bordes del rectángulo se busca
// - b_side: Tamaño del lado del cuadrado mayor
// - l_side: Tamaño del lado del cuadrado menor extraído
std::vector<Point_2> inter_border(Segment_2 const& s, float b_side, float l_side) {
    // se tienen los segmentos que representan el borde de la L
    Segment_2 b_b(Point_2(0.0f, 0.0f), Point_2(b_side, 0.0f));
    Segment_2 b_r(Point_2(b_side, 0.0f), Point_2(b_side, b_side-l_side));
    Segment_2 l_b(Point_2(b_side, b_side-l_side), Point_2(b_side-l_side, b_side-l_side));
    Segment_2 l_l(Point_2(b_side-l_side, b_side-l_side), Point_2(b_side-l_side, b_side));
    Segment_2 b_t(Point_2(b_side-l_side, b_side), Point_2(0.0f, b_side));
    Segment_2 b_l(Point_2(0.0f, b_side), Point_2(0.0f, 0.0f));
    // vector con bordes
    std::vector<Segment_2> segs = {b_b, b_r, l_b, l_l, b_t, b_l};

    // vector para almacenar intersecciones
    std::vector<Point_2> result;
    for (int i = 0; i < segs.size(); i++) {
        auto res = CGAL::intersection(s, segs[i]);
        if (res) {
            // si interseccion de s con el borde es un punto, se agrega
            if (const Point_2* ip = boost::get<Point_2>(&*res)) {
                result.push_back(*ip);
            // si es un segmento, se agregan los extremos
            } else if (const Segment_2* sp = boost::get<Segment_2>(&*res)) {
                result.push_back(sp->source());
                result.push_back(sp->target());
            }
        }
    }
    return result;

}

// Recorta y extrae el segmento para que esté dentro del rectángulo
// Parámetros:
// - s: Segmento a recortar
// - b_side: Tamaño del lado del cuadrado mayor
// - l_side: Tamaño del lado del cuadrado menor extraído
Segment_2 crop_and_extract_segment(const Segment_2& s, float b_side, float l_side) {
    Point_2 src = s.source();
    Point_2 tgt = s.target();

    // si ambos extremos están dentro, se retorna para colocar ambos puntos
    if (is_point_in_L(src, b_side, l_side) && is_point_in_L(tgt, b_side, l_side)) {
        return s;
    // si solo el punto final está fuera, se retorna el segmento formado por el punto inicial
    // y el punto de intersección con el borde
    } else if (is_point_in_L(src, b_side, l_side) && !is_point_in_L(tgt, b_side, l_side)) {
        std::vector<Point_2> inters =inter_border(s, b_side, l_side);
        if (inters.size() == 1) {
            Point_2 intersection_point = inters.front();
            return Segment_2(src, intersection_point);
        } else return Segment_2(src, src);
    // si solo el punto final está dentro, se retorna el segmento formado por el punto de intersección con el borde
    // y el punto final
    } else if (!is_point_in_L(src, b_side, l_side) && is_point_in_L(tgt, b_side, l_side)) {
        std::vector<Point_2> inters =inter_border(s, b_side, l_side);
        if (inters.size() == 1) {
            Point_2 intersection_point = inters.front();
            return Segment_2(intersection_point, tgt);
        } else return Segment_2(tgt, tgt);
    // en caso contrario ambos están afuera y se retorna un segmento/señal
    } else return Segment_2(Point_2(0, 0), Point_2(0, 0));

}

// Extrae segmentos válidos de un conjunto de puntos dentro del rectángulo
// Parámetros:
// - pts: Vector de puntos que forman los segmentos a validar
// - b_side: Tamaño del lado del cuadrado mayor
// - l_side: Tamaño del lado del cuadrado menor extraído
std::vector<Point_2> extractValidSegments(std::vector<Point_2>& pts, float b_side, float l_side) {
    std::vector<Point_2> cropped_segments;
        // por cada par de puntos se revisan sus intersecciones
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