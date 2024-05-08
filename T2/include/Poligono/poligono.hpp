#include<iostream>
#include <vector>
#include <stdexcept>
#include<cmath>
#include "../Vector/vector.hpp"

template <typename T>
class Poligono {
    private:
        std::vector<Punto<T>> vec;
        int count;

    public:
        Poligono(std::vector<Punto<T>> v) {
            count = 0;
            for (Punto<T> punto : v) {
                vec.push_back(punto);
                count++;
            }
        }

        int GetPointsAmount() const {
            return this->count;
        }

        void addPoint(Punto<T> punto) {
            vec.push_back(punto);
            count++;
        }

        Punto<T>& operator[](int i) {
            if (i < count && i >= 0) return this->vec[i];
            else throw std::out_of_range("Entero fuera del rango admitido");
        }
        bool PointIsIn(Punto<T> punto) {
            for (int i = 0; i < count; i++) {
                if (punto == vec[i]) return true;
            }
            return false;
        }

        bool isCounterclockwise() {
            double area = 0;
            int c = this->count;
            for (int i = 0; i < c; i++) {
                int j = (i+1)%c;
                Vector<T> v1(vec[i]), v2(vec[j]);
                area += v1.CrossProduct(v2);
            }
            return area > 0;
        }

        friend std::ostream& operator<<(std::ostream& out, const Poligono<T>& p) {
            out << "[";
            int c = p.count;
            for (int i = 0; i < c; i++) {
                if (i != c - 1) out << "(" << p.vec[i].GetX() << ", " << p.vec[i].GetY() << ")" << ",";
                else out << "(" << p.vec[i].GetX() << ", " << p.vec[i].GetY() << ")" << "]";
            }
            return out;
        }

};