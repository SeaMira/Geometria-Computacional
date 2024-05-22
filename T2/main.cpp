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

Times t_gw;
Times t_gs;

int main(int argc, char const *argv[])
{
    using std::chrono::microseconds;
    std::filesystem::path pth = std::filesystem::current_path();
    std::cout << pth << std::endl;

    // CSV de los experimentos de Gift Wrapping
    std::ofstream gw_file("gift_wrapping_results.csv"); // Open the file for writing
    if (!gw_file.is_open()) {
        std::cerr << "Failed to open 'gift_wrapping_results' file\n";
        return 1;
    }
    // CSV de los experimentos de Graham Scan
    std::ofstream gs_file("graham_scan_results.csv"); // Open the file for writing
    if (!gs_file.is_open()) {
        std::cerr << "Failed to open 'graham_scan_results' file\n";
        return 1;
    }

    std::ofstream debug_gw_file("poligons_gw.csv"); // Open the file for writing
    if (!debug_gw_file.is_open()) {
        std::cerr << "Failed to open 'debug_file' file\n";
        return 1;
    }

    std::ofstream debug_gs_file("poligons_gs.csv"); // Open the file for writing
    if (!debug_gs_file.is_open()) {
        std::cerr << "Failed to open 'debug_file' file\n";
        return 1;
    }
    
    int size = 10;
    for (int i = 0; i < 3; i++) {
        
        for (int times = 0; times < 1; times++) {
            std::cout << "Time " << times+1 << " Size " << size << std::endl;
            float range = 100;
            Punto<float>* p = nPointList(size, range);

            auto t_start = std::chrono::high_resolution_clock::now();
            Poligono<float> pol_gw = giftWrapping(p, size);
            auto t_end = std::chrono::high_resolution_clock::now();
            t_gw.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

            t_start = std::chrono::high_resolution_clock::now();
            Poligono<float> pol_gs = grahamScan(p, size);
            t_end = std::chrono::high_resolution_clock::now();
            t_gs.execution = std::chrono::duration_cast<microseconds>(t_end - t_start).count();

            bool poligon_comparison_result = equalPolygons(pol_gw, pol_gs);
            // los csv serán de la forma n , tiempo, cantidad_vertices, igualdad_resultados (por ahora...)
            gw_file << size << "," << t_gw.execution << "," << pol_gw.GetPointsAmount() << "," << poligon_comparison_result << "\n";
            gs_file << size << "," << t_gs.execution << "," << pol_gs.GetPointsAmount() << "," << poligon_comparison_result << "\n";

            if (!poligon_comparison_result) {
                for (int i = 0; i < pol_gw.GetPointsAmount(); i++) {
                    debug_gw_file << pol_gw[i].GetX() << "," << pol_gw[i].GetY() << "\n";
                }

                for (int i = 0; i < pol_gs.GetPointsAmount(); i++) {
                    debug_gs_file << pol_gs[i].GetX() << "," << pol_gs[i].GetY() << "\n";
                }
            }

        }
        size *= 10;
    }

    gw_file.close();
    gs_file.close();
    debug_gw_file.close();
    debug_gs_file.close();
    return 0;

}
