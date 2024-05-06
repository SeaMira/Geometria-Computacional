#include<iostream>
#include<cmath>
#include <string>

template <typename T>
class Punto {
    private:
        std::pair<T, T> par;
    public:

        Punto(T x, T y) {
            this->par.first = x; this->par.second = y;
        }

        ~Punto() {
            std::cout<<"Deleted Punto"<<std::endl;
        }

        void SetX(T x) {
            par.first = x; 
        }

        void SetY(T y) {
            par.second = y; 
        }

        T GetX() const {
            return par.first;
        }

        T GetY() const {
            return par.second;
        }

        std::string ToString() const {
        return "(" + std::to_string(GetX()) + ", " + std::to_string(GetY()) + ")";
        }   

        double DistToPoint(const Punto<T>& p) {
            T comp1 = pow(p.GetX() - this->GetX(), 2);
            T comp2 = pow(p.GetY() - this->GetY(), 2);
            return sqrt(comp1 + comp2);
        }

        bool operator==(const Punto<T>& p) const {
            return (this->GetX() == p.GetX()) && (this->GetY() == p.GetY());
        } 

        friend std::ostream& operator<<(std::ostream& out, const Punto<T>& p) {
            out << "(" << p.GetX() << " , " << p.GetY() << ")";
            return out;
        }

};

