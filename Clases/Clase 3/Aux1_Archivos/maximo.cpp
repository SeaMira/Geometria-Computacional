#include <iostream>
#include <string>

template <typename T>
T maximo(T x, T y) {
    return (x > y) ? x : y;
}

int main() {
    int a = 5, b = 10;
    std::cout << "El maximo entre " << a << " y " << b << " es: " << maximo(a, b) << std::endl;

    double c = 3.14, d = 2.71;
    std::cout << "El maximo entre " << c << " y " << d << " es: " << maximo(c, d) << std::endl;

    std::string s1 = "hola", s2 = "mundo";
    std::cout << "El maximo entre \"" << s1 << "\" y \"" << s2 << "\" es: " << maximo(s1, s2) << std::endl;

    return 0;
}
