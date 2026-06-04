#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <iostream>
#include <vector>
#include <string>
class Complex {
private:
    double real, img;
public:
    Complex(double r = 0.0, double i = 0.0): real(r), img(i) {}
    Complex operator+(const Complex & c);
    Complex operator+(int a);
    Complex & operator+=(const Complex & c);
    friend Complex operator+(int a, const Complex & c);
    friend std::ostream & operator<<(std::ostream & o, const Complex & c);
    friend std::istream & operator>>(std::istream & i, Complex & c);
};

#endif
