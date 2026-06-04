#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include "Complex.h"

using namespace std;

Complex Complex::operator+(const Complex & c) {
    return Complex(real + c.real, img + c.img);
}

Complex Complex::operator+(int a) {
    return Complex(real + static_cast<double>(a), img); 
}

Complex & Complex::operator+=(const Complex & c) {
    real = real + c.real;
    img = img + c.img;
    return *this;        
}

Complex operator+(int a, const Complex & c) {
    return Complex(c.real + static_cast<double>(a), c.img);
}

ostream & operator<<(ostream & o, const Complex & c) {
    if (c.img > 0) {
        o << c.real << "+" << c.img << "i";
    }
    else if (c.img == 0) {
        o << c.real;
    }
    else {
        o << c.real << c.img << "i";
    }
    return o;
}

static vector<double> getc(string s) {
    double d = 0.0;
    vector<double> D;
    string word = "";
    for(size_t i = 0; i < s.length(); ++i) {
        if(s[i] == '+' || s[i] == 'i' || s[i] == 'I') {
            d = stod(word);
            D.push_back(d);
            d = 0.0;
            word.clear();
        }
        else if(s[i] == ' ') {
            continue;
        }
        else {
            word.push_back(s[i]);
        }
    }
    return D;
}

istream & operator>>(istream & i, Complex & c) {
    string w = "";
    getline(i, w);
    vector<double> D = getc(w);
    if (D.size() != 2) {
        cout << "无效输入" << endl;
        return i;
    }
    c.real = D[0];
    c.img = D[1];
    return i;
}