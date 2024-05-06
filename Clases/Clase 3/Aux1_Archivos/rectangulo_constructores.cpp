#include <iostream>
#include <vector>

class Rectangulo {
private:
    double ancho;
    double alto;

public:
    Rectangulo() {
        ancho = 0;
        alto = 0;
        std::cout << "Constructor por defecto" << std::endl;
    }

    Rectangulo(double ancho, double alto) {
        this->ancho = ancho;
        this->alto = alto;
        std::cout << "Constructor con parametros" << std::endl;
    }

    Rectangulo(const Rectangulo& rect) {
        this->ancho = rect.ancho;
        this->alto = rect.alto;
        std::cout << "Constructor de copia" << std::endl;
    }

    Rectangulo(Rectangulo&& rect) {
        this->ancho = rect.ancho;
        this->alto = rect.alto;
        rect.ancho = 0;
        rect.alto = 0;
        std::cout << "Constructor de movimiento" << std::endl;
    }

    ~Rectangulo() {
        std::cout << "Destructor" << std::endl;
    }

    double calcular_area() const {
        return ancho * alto;
    }
};

int main() {
    std::vector<Rectangulo> rectangulos;
    rectangulos.push_back(Rectangulo(2, 3));
    rectangulos.push_back(Rectangulo(4, 5));
    rectangulos.push_back(Rectangulo(6, 7));

    for (const Rectangulo& rect : rectangulos) {
        std::cout << "Area del rectangulo: " << rect.calcular_area() << std::endl;
    }

    std::cout<< rectangulos.at(2).calcular_area() << std::endl;
    rectangulos[50];

    return 0;
}
