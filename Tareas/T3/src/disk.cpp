#include <Disk/disk.hpp>

void removeDuplicates(std::vector<Point_2>& myVector);


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



void DiskTriangulation::compute_voronoi() {
    for (auto vit = disk.finite_vertices_begin(); vit != disk.finite_vertices_end(); ++vit) {
        std::vector<Point_2> circ_centers;

        
        CDT::Face_circulator fcirc = disk.incident_faces(vit), done(fcirc);
        if (fcirc != 0) {
            do {
                if (!disk.is_infinite(fcirc)) {
                    Point_2 circ_center = CGAL::circumcenter(disk.triangle(fcirc));
                    
                    
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

        voronoi_segments.push_back(circ_centers);

        
    }
}



DiskTriangulation::DiskTriangulation(float radius, int points, float pcnt) : radius(radius), points(points), pcnt(pcnt) {
    getCircleTriangulation(disk, radius, points, pcnt);
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

