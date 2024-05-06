#include <iostream>

class Complejo {
private:
    double real, imag;

public:
    Complejo(double r = 0, double i = 0) : real(r), imag(i) {}

    Complejo operator+(Complejo const &obj) {
        Complejo res;
        res.real = real + obj.real;
        res.imag = imag + obj.imag;
        return res;
    }

    void imprimir() {
        std::cout << real << " + " << imag << "i" << std::endl;
    }
};

int main() {
    Complejo a(3.0, 4.0);
    Complejo b(2.0, 3.0);
    Complejo c = a + b;
    c.imprimir();
    return 0;
}
