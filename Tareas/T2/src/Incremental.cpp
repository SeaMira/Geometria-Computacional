#include "GrahamScan.cpp"

struct TimesIN {
  long sorting, lower, upper;
  long total() { return sorting+lower+upper; }
};

float cross(const Punto<float>& O, const Punto<float>& A, const Punto<float>& B) {
    return (A.GetX() - O.GetX()) * (B.GetY() - O.GetY()) - (A.GetY() - O.GetY()) * (B.GetX() - O.GetX());
}

Poligono<float> Incremental(Punto<float> puntos[], int n) {
    int k = 0;

    std::vector<Punto<float>> P(puntos, puntos + n);
    std::sort(P.begin(), P.end());

    std::vector<Punto<float>> H(2 * n);

    // Construir el lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    // Construir el upper hull
    for (int i = n - 1, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }

    H.resize(k - 1);
    return Poligono<float>(H);
}


TimesIN t_in;

Poligono<float> IncrementalRegister(Punto<float> puntos[], int n, char *filename) {
    using std::chrono::microseconds;
    int k = 0;

    std::ofstream in_file(filename, std::ios::app); // Open the file for writing
    if (!in_file.is_open()) {
        std::cerr << "Failed to open file\n";
    }

    std::vector<Punto<float>> P(puntos, puntos + n);

    auto t_start = std::chrono::high_resolution_clock::now();
    std::sort(P.begin(), P.end());
    auto t_end = std::chrono::high_resolution_clock::now();
    t_in.sorting = std::chrono::duration_cast<microseconds>(t_end - t_start).count();
    std::vector<Punto<float>> H(2 * n);

    t_start = std::chrono::high_resolution_clock::now();
    // Construir el lower hull
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }
    t_end = std::chrono::high_resolution_clock::now();
    t_in.lower = std::chrono::duration_cast<microseconds>(t_end - t_start).count();
    
    t_start = std::chrono::high_resolution_clock::now();
    // Construir el upper hull
    for (int i = n - 1, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(H[k - 2], H[k - 1], P[i]) <= 0) k--;
        H[k++] = P[i];
    }
    t_end = std::chrono::high_resolution_clock::now();
    t_in.upper = std::chrono::duration_cast<microseconds>(t_end - t_start).count();
    in_file << n << "," << t_in.sorting << "," << t_in.lower << "," << t_in.upper << "," << t_in.total() << "," << k-1 <<  "\n";
    in_file.close();

    H.resize(k - 1);
    return Poligono<float>(H);
}