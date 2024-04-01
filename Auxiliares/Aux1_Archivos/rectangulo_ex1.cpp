#include <iostream>

class Rectangulo {
private:
    // atributos privados
    double ancho, alto;
public:
    // Constructor por defecto
    Rectangulo() {
        ancho = 0.0; alto = 0.0; }

    // Constructor con parámetros
    Rectangulo(double ancho, double alto) {
        this->ancho = ancho; this->alto = alto; }

    // método público para calcular el área
    double calcular_area(){
        return ancho * alto;}
};

int main()
{
    //Crea un objeto en el stack
    Rectangulo fig1 = Rectangulo(1,2);
    std::cout<<fig1.calcular_area()<<std::endl;

    //Crea un objecto en el heap
    Rectangulo *fig2 = new Rectangulo(4, 5);
    std::cout<<fig2->calcular_area()<<std::endl;
    delete fig2;
    


    return 0;
}