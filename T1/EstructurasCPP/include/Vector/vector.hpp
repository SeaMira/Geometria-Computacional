#include<iostream>
#include<cmath>
#include "../Punto/punto.hpp"

template <typename T>
class Vector {
    private:
        T x, y;
    public:
        Vector(T x, T y) {
            this->x = x;
            this->y = y;
        }

        Vector(Punto<T>& p) {
            this->x = p.GetX();
            this->y = p.GetY();
        }

        T GetX() const {
            return this->x;
        }

        T GetY() const {
            return this->y;
        }

        void SetX(T x) {
            this->x = x;
        }

        void SetY(T y) {
            this->y = y;
        }

        double Size() {
            return sqrt(pow(this->x, 2) + pow(this->y, 2));
        }

        T PointProduct(const Vector<T>& v) {
            return this->GetX() * v.GetX() + this->GetY() * v.GetY();
        }

        T CrossProduct(const Vector<T>& v) {
            return this->GetX() * v.GetY() - this->GetY() * v.GetX();
        }

        Vector<T> operator+(const Vector<T>& other) {
            Vector<T> vec(this->GetX()+other.GetX(), this->GetY()+other.GetY());
            return vec;
        }

        bool operator==(const Vector<T>& other) const {
            return (this->GetX() == other.GetX()) && (this->GetY() == other.GetY());
        }

        Vector<T> operator*(T scalar) {
            Vector<T> vec(this->GetX()*scalar, this->GetY()*scalar);
            return vec;
        }

        friend std::ostream& operator<<(std::ostream& out, const Vector<T>& v) {
            out << "(" << v.GetX() << " , " << v.GetY() << ")";
            return out;
        }
};