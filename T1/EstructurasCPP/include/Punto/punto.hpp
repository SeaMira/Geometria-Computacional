#include<iostream>
#include<cmath>

template <typename T>
class punto {
    private:
        std::pair<T, T> par;
    public:

        punto(T x, T y) {
            this->par.first = x; this->par.second = y;
        }

        ~punto() {
            std::cout<<"Deleted punto"<<std::endl;
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

        double DistToPoint(const punto<T>& p) {
            T comp1 = pow(p.GetX() - this->GetX(), 2);
            T comp2 = pow(p.GetY() - this->GetY(), 2);
            return sqrt(comp1 + comp2);
        }

        bool operator==(const punto<T>& p) {
            return (this->GetX() == p.GetX()) && (this->GetY() == p.GetY());
        } 

        friend std::ostream& operator<<(std::ostream& out, const punto<T>& p) {
            out << "(" << p.GetX() << " , " << p.GetY() << ")";
            return out;
        }

};

