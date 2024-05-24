#include<iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
// #include"src/ExperimentFunctions.cpp"
#include"src/GrahamScan.cpp"
// #include"Punto/punto.hpp"
// #include"Vector/vector.hpp"

struct Times {
  long execution;
  long total() { return execution; }
};

Times t;


bool test_random_points_gw(int size, float range) {
    using std::chrono::microseconds;

    // CSV de los experimentos de Graham Scan
    std::ofstream gw_file("gift_wrapping_results.csv", std::ios::app); // Open the file for writing
    if (!gw_file.is_open()) {
        std::cerr << "Failed to open 'gift_wrapping_results' file\n";
        return false;
    }

    Punto<float>* p = nPointList(size, range);

    auto t_start = std::chrono::high_resolution_clock::now();
    Poligono<float> pol_gw = giftWrapping(p, size);
    auto t_end = std::chrono::high_resolution_clock::now();
    t.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    gw_file << size << "," << t.execution << "," << pol_gw.GetPointsAmount() <<  "\n";

    std::ofstream debug_gw_file("poligons_gw.csv"); // Open the file for writing
    if (!debug_gw_file.is_open()) {
        std::cerr << "Failed to open 'debug_file' file\n";
        return false;
    }

    for (int i = 0; i < pol_gw.GetPointsAmount(); i++) {
        debug_gw_file << pol_gw[i].GetX() << "," << pol_gw[i].GetY() << "\n";
    }

    gw_file.close();
    debug_gw_file.close();
    return true;
}


bool test_random_points_gs(int size, float range) {
    using std::chrono::microseconds;

    // CSV de los experimentos de Gift Wrapping
    std::ofstream gs_file("graham_scan_results.csv", std::ios::app); // Open the file for writing
    if (!gs_file.is_open()) {
        std::cerr << "Failed to open 'graham_scan_results' file\n";
        return false;
    }
    
    Punto<float>* p = nPointList(size, range);

    auto t_start = std::chrono::high_resolution_clock::now();
    Poligono<float> pol_gs = grahamScan(p, size);
    auto t_end = std::chrono::high_resolution_clock::now();
    t.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    // los csv serán de la forma n , tiempo, cantidad_vertices
    gs_file << size << "," << t.execution << "," << pol_gs.GetPointsAmount() <<  "\n";


    std::ofstream debug_gs_file("poligons_gs.csv"); // Open the file for writing
    if (!debug_gs_file.is_open()) {
        std::cerr << "Failed to open 'debug_file' file\n";
        return false;
    }

    for (int i = 0; i < pol_gs.GetPointsAmount(); i++) {
        debug_gs_file << pol_gs[i].GetX() << "," << pol_gs[i].GetY() << "\n";
    }
    

    gs_file.close();
    debug_gs_file.close();
    return true;
}


bool test_extrn_points_gw(int size, float radius, float prcnt) {
    using std::chrono::microseconds;

    // CSV de los experimentos de Gift Wrapping
    std::ofstream gw_file_extern_pts("gift_wrapping_extrn_pts_results.csv", std::ios::app); // Open the file for writing
    if (!gw_file_extern_pts.is_open()) {
        std::cerr << "Failed to open 'gift_wrapping_extrn_pts_results' file\n";
        return false;
    }
    
    Punto<float>* p = pcntConvexFromNPoints(size, prcnt, radius);

    auto t_start = std::chrono::high_resolution_clock::now();
    Poligono<float> pol_gw = giftWrapping(p, size);
    auto t_end = std::chrono::high_resolution_clock::now();
    t.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    // los csv serán de la forma n , tiempo, cantidad_vertices
    gw_file_extern_pts << size << "," << t.execution << "," << pol_gw.GetPointsAmount() << "," << prcnt  <<  "\n";


    std::ofstream debug_gw_file("poligons_gw_extrn_pts.csv"); // Open the file for writing
    if (!debug_gw_file.is_open()) {
        std::cerr << "Failed to open 'poligons_gw_extrn_pts' debug file\n";
        return false;
    }

    for (int i = 0; i < pol_gw.GetPointsAmount(); i++) {
        debug_gw_file << pol_gw[i].GetX() << "," << pol_gw[i].GetY() << "\n";
    }
    

    gw_file_extern_pts.close();
    debug_gw_file.close();
    return true;
}


bool test_extrn_points_gs(int size, float radius, float prcnt) {
    using std::chrono::microseconds;

    // CSV de los experimentos de Gift Wrapping
    std::ofstream gs_file_extern_pts("graham_scan_extrn_pts_results.csv", std::ios::app); // Open the file for writing
    if (!gs_file_extern_pts.is_open()) {
        std::cerr << "Failed to open 'graham_scan_extrn_pts_results' file\n";
        return false;
    }
    
    Punto<float>* p = pcntConvexFromNPoints(size, prcnt, radius);

    auto t_start = std::chrono::high_resolution_clock::now();
    Poligono<float> pol_gs = grahamScan(p, size);
    auto t_end = std::chrono::high_resolution_clock::now();
    t.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

    // los csv serán de la forma n , tiempo, cantidad_vertices
    gs_file_extern_pts << size << "," << t.execution << "," << pol_gs.GetPointsAmount() << "," << prcnt <<  "\n";


    std::ofstream debug_gs_file("poligons_gs_extrn_pts.csv"); // Open the file for writing
    if (!debug_gs_file.is_open()) {
        std::cerr << "Failed to open 'poligons_gs_extrn_pts' debug file\n";
        return false;
    }

    for (int i = 0; i < pol_gs.GetPointsAmount(); i++) {
        debug_gs_file << pol_gs[i].GetX() << "," << pol_gs[i].GetY() << "\n";
    }
    

    gs_file_extern_pts.close();
    debug_gs_file.close();
    return true;
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
    std::cout << n << " " << radius_range << " " << pcnt << std::endl;

    switch (mode)
    {
    case 1:
        if (!test_random_points_gw(n, radius_range)) {
            std::cerr << "Error while executing the gift wrapping algorithm on random points" << std::endl;
            return 3;
        }
        break;
    case 2:
        if (!test_random_points_gs(n, radius_range)) {
            std::cerr << "Error while executing the graham scan algorithm on random points" << std::endl;
            return 3;
        }
        break;
    case 3:
        if (!test_extrn_points_gw(n, radius_range, pcnt)) {
            std::cerr << "Error while executing the gift wrapping algorithm on with a porcentage of points on a circle" << std::endl;
            return 3;
        }
        break;
    case 4:
        if (!test_extrn_points_gs(n, radius_range, pcnt)) {
            std::cerr << "Error while executing the graham scan algorithm on with a porcentage of points on a circle" << std::endl;
            return 3;
        }
        break;
  
    default:
        break;
  }

    return 0;

}
