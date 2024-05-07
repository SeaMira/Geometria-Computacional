class Rectangulo {
private:
    // atributos privados
    double ancho;
    double alto;

public:
    
    // Constructor por defecto
    Rectangulo();

    // Constructor con parámetros
    Rectangulo(float ancho, float alto);

    // método público para calcular el área
    double calcular_area();
};
