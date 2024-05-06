#include <iostream>

class complejo {
public:
    complejo(double real, double imag);
    double getReal() const;
    double getImag() const;
    void setReal(double real);
    void setImag(double imag);
    complejo operator+(const complejo& c) const;
    complejo operator*(const complejo& c) const;
    friend std::ostream& operator<<(std::ostream& os, const complejo& c);
    friend std::istream& operator>>(std::istream& is, complejo& c);

private:
    double real_;
    double imag_;
};

complejo::complejo(double real, double imag) : real_(real), imag_(imag) {}

double complejo::getReal() const {
    return real_;
}

double complejo::getImag() const {
    return imag_;
}

void complejo::setReal(double real) {
    real_ = real;
}

void complejo::setImag(double imag) {
    imag_ = imag;
}

complejo complejo::operator+(const complejo& c) const {
    return complejo(real_ + c.real_, imag_ + c.imag_);
}

complejo complejo::operator*(const complejo& c) const {
    return complejo(real_ * c.real_ - imag_ * c.imag_, real_ * c.imag_ + imag_ * c.real_);
}

std::ostream& operator<<(std::ostream& os, const complejo& c) {
    os << c.real_ << " + " << c.imag_ << "i";
    return os;
}

std::istream& operator>>(std::istream& is, complejo& c) {
    is >> c.real_ >> c.imag_;
    return is;
}

int main() {
    complejo c1(3.0, 4.0);
    complejo c2(2.0, -1.0);

    std::cout << "c1 = " << c1 << std::endl;
    std::cout << "c2 = " << c2 << std::endl;

    complejo c3 = c1 + c2;
    std::cout << "c3 = " << c3 << std::endl;

    return 0;
}