#include<iostream>

#include <filesystem>
// #include"src/ExperimentFunctions.cpp"
#include"src/Incremental.cpp"
// #include"Punto/punto.hpp"
// #include"Vector/vector.hpp"

struct Times {
  long execution;
  long total() { return execution; }
};

Times t;


Poligono<float> test_random_points_gw(Punto<float>* p, int size, char *filename) {
    using std::chrono::microseconds;

    Poligono<float> pol_gw = giftWrappingRegister(p, size, filename);

    std::ofstream debug_gw_file("poligons_gw.csv"); // Open the file for writing
    if (!debug_gw_file.is_open()) {
        std::cerr << "Failed to open 'debug_file' file\n";
        return pol_gw;
    }

    for (int i = 0; i < pol_gw.GetPointsAmount(); i++) {
        debug_gw_file << pol_gw[i].GetX() << "," << pol_gw[i].GetY() << "\n";
    }

    debug_gw_file.close();
    return pol_gw;
}

Poligono<float> test_random_points_in(Punto<float>* p, int size, char *filename) {

    Poligono<float> pol_in = IncrementalRegister(p, size, filename);

    std::ofstream debug_incremental_file("poligons_in.csv"); // Open the file for writing
    if (!debug_incremental_file.is_open()) {
        std::cerr << "Failed to open 'poligons_in' file\n";
        return pol_in;
    }

    for (int i = 0; i < pol_in.GetPointsAmount(); i++) {
        debug_incremental_file << pol_in[i].GetX() << "," << pol_in[i].GetY() << "\n";
    }

    debug_incremental_file.close();
    return pol_in;
}


Poligono<float> test_extrn_points_gw(Punto<float>* p, int size, char *filename) {
    
    Poligono<float> pol_gw = giftWrappingRegister(p, size, filename);

    std::ofstream debug_gw_file("poligons_gw_extrn_pts.csv"); // Open the file for writing
    if (!debug_gw_file.is_open()) {
        std::cerr << "Failed to open 'poligons_gw_extrn_pts' debug file\n";
        return pol_gw;
    }

    for (int i = 0; i < pol_gw.GetPointsAmount(); i++) {
        debug_gw_file << pol_gw[i].GetX() << "," << pol_gw[i].GetY() << "\n";
    }
    
    debug_gw_file.close();
    return pol_gw;
}


Poligono<float> test_extrn_points_in(Punto<float>* p, int size, char *filename) {
    
    Poligono<float> pol_in = IncrementalRegister(p, size, filename);

    std::ofstream debug_in_file("poligons_in_extrn_pts.csv"); // Open the file for writing
    if (!debug_in_file.is_open()) {
        std::cerr << "Failed to open 'poligons_in_extrn_pts' debug file\n";
        return pol_in;
    }

    for (int i = 0; i < pol_in.GetPointsAmount(); i++) {
        debug_in_file << pol_in[i].GetX() << "," << pol_in[i].GetY() << "\n";
    }
    
    debug_in_file.close();
    return pol_in;
}

// Poligono<float> test_random_points_gs(Punto<float>* p, int size, float range) {
//     using std::chrono::microseconds;

//     // CSV de los experimentos de Gift Wrapping
//     std::ofstream gs_file("graham_scan_results.csv", std::ios::app); // Open the file for writing
//     if (!gs_file.is_open()) {
//         std::cerr << "Failed to open 'graham_scan_results' file\n";
//     }
    
//     auto t_start = std::chrono::high_resolution_clock::now();
//     Poligono<float> pol_gs = grahamScan(p, size);
//     auto t_end = std::chrono::high_resolution_clock::now();
//     t.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

//     // los csv serán de la forma n , tiempo, cantidad_vertices
//     gs_file << size << "," << t.execution << "," << pol_gs.GetPointsAmount() <<  "\n";


//     std::ofstream debug_gs_file("poligons_gs.csv"); // Open the file for writing
//     if (!debug_gs_file.is_open()) {
//         std::cerr << "Failed to open 'debug_file' file\n";
//         return pol_gs;
//     }

//     for (int i = 0; i < pol_gs.GetPointsAmount(); i++) {
//         debug_gs_file << pol_gs[i].GetX() << "," << pol_gs[i].GetY() << "\n";
//     }
    

//     gs_file.close();
//     debug_gs_file.close();
//     return pol_gs;
// }

// Poligono<float> test_extrn_points_gs(Punto<float>* p, int size, float radius, float prcnt) {
//     using std::chrono::microseconds;

//     // CSV de los experimentos de Gift Wrapping
//     std::ofstream gs_file_extern_pts("graham_scan_extrn_pts_results.csv", std::ios::app); // Open the file for writing
//     if (!gs_file_extern_pts.is_open()) {
//         std::cerr << "Failed to open 'graham_scan_extrn_pts_results' file\n";
//     }

//     auto t_start = std::chrono::high_resolution_clock::now();
//     Poligono<float> pol_gs = grahamScan(p, size);
//     auto t_end = std::chrono::high_resolution_clock::now();
//     t.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

//     // los csv serán de la forma n , tiempo, cantidad_vertices
//     gs_file_extern_pts << size << "," << t.execution << "," << pol_gs.GetPointsAmount() << "," << prcnt <<  "\n";


//     std::ofstream debug_gs_file("poligons_gs_extrn_pts.csv"); // Open the file for writing
//     if (!debug_gs_file.is_open()) {
//         std::cerr << "Failed to open 'poligons_gs_extrn_pts' debug file\n";
//         return pol_gs;
//     }

//     for (int i = 0; i < pol_gs.GetPointsAmount(); i++) {
//         debug_gs_file << pol_gs[i].GetX() << "," << pol_gs[i].GetY() << "\n";
//     }
    

//     gs_file_extern_pts.close();
//     debug_gs_file.close();
//     return pol_gs;
// }



void test_comparison_random(int n, int radius_range) {
    Punto<float>* p = nPointList(n, radius_range);
    char gw_filename[] = "random_point_gw.csv";
    Poligono<float> gw = test_random_points_gw(p, n, gw_filename);
    // Poligono<float> gs = test_random_points_gs(p, n, radius_range);
    char in_filename[] = "random_point_in.csv";
    Poligono<float> in = test_random_points_in(p, n, in_filename);
    // bool equality_gw_gs = equalPolygons(gw, gs);
    // bool equality_gs_in = equalPolygons(in, gs);
    bool equality_gw_in = equalPolygons(gw, in);

    std::ofstream comparison_file("comparison_random_pnts_file.csv", std::ios::app); // Open the file for writing
    if (!comparison_file.is_open()) {
        std::cerr << "Failed to open 'comparison_random_pnts_file' file\n";
        return ;
    }

    // los csv serán de la forma n , tiempo, cantidad_vertices
    comparison_file << n << "," << gw.GetPointsAmount() << "," << in.GetPointsAmount() << "," << equality_gw_in << "\n";

    comparison_file.close();
}

void test_comparison_circ(int n, float pcnt, int radius_range) {
    Punto<float>* p = pcntConvexFromNPoints(n, pcnt, radius_range);
    char gw_filename[] = "circle_point_gw.csv";
    Poligono<float> gw = test_extrn_points_gw(p, n, gw_filename);
    // Poligono<float> gs = test_extrn_points_gs(p, n, radius_range, pcnt);
    char in_filename[] = "circle_point_in.csv";
    Poligono<float> in = test_extrn_points_in(p, n, in_filename);
    // bool equality_gw_gs = equalPolygons(gw, gs);
    // bool equality_gs_in = equalPolygons(in, gs);
    bool equality_gw_in = equalPolygons(gw, in);

    std::ofstream comparison_file("comparison_circ_pnts_file.csv", std::ios::app); // Open the file for writing
    if (!comparison_file.is_open()) {
        std::cerr << "Failed to open 'comparison_circ_pnts_file' file\n";
        return ;
    }

    // los csv serán de la forma n , tiempo, cantidad_vertices
    comparison_file << n << "," << gw.GetPointsAmount() << "," << in.GetPointsAmount() << "," << pcnt << "," << equality_gw_in << "\n";

    comparison_file.close();
}



int main(int argc, char const *argv[]) {

    if (argc != 5) {
    std::cerr << "Uso: " << argv[0]
                << " <mode> <size> <radius/range> <prcnt> "
                << std::endl;
    return 2;
    }
    int mode = std::stoi(argv[1]);
    int n = std::stoi(argv[2]);
    int radius_range = std::stoi(argv[3]);
    float pcnt = std::stof(argv[4]);

    // Punto<float>* p = pcntConvexFromNPoints(n, pcnt, radius_range);
    // for (int i = 0; i < n; i++) std::cout << p[i] << std::endl;
    // Poligono<float> pol_gs = grahamScan(p, n);
    // for (int i = 0; i < n; i++) std::cout << p[i] << std::endl;

    switch (mode)
    {
    case 1:
        test_comparison_random(n, radius_range);
        break;
    case 2:
        test_comparison_circ(n, pcnt, radius_range);
        break;        
    default:
        break;
  }

    return 0;

}
