#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};
    
    // utilizando iteradores
    std::cout << "Iterando con iteradores: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // utilizando un rango-for loop
    std::cout << "Iterando con un rango-for loop: ";
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    

    // Modificamos el contenido del vector usando iteradores
    std::cout << "Vector modificado: ";
    for (auto it = v.begin(); it != v.end(); ++it) {
        *it = *it * 2;
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
