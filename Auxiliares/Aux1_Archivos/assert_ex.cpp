#include <iostream>
#include <cassert>

int main() {
    int a = 5;
    int b = 3;
    assert(a < b);

    std::cout << "La aserción se cumplió correctamente." << std::endl;
    return 0;
}