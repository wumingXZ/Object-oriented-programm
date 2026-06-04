#ifndef _STRING_H
#define _STRING_H

#include <algorithm>
#include <iostream>
#include <cctype>
#include "Vec.h"

class Str {
public:
    typedef Vec<char>::size_type size_type;

    Str() {}
    Str(size_type n, char c) : data(n, c) {}
    Str(const char* cp);

    size_type size() const { return data.size(); }

    char& operator[](size_type i) { return data[i]; }
    const char& operator[](size_type i) const { return data[i]; }

    Str& operator+=(const Str& rhs);
    Str& operator+=(const char* rhs);
    Str& operator+=(char c);

private:
    Vec<char> data;
};

Str operator+(const Str& lhs, const Str& rhs);
Str operator+(const Str& lhs, const char* rhs);
Str operator+(const char* lhs, const Str& rhs);

std::ostream& operator<<(std::ostream& os, const Str& s);
std::istream& operator>>(std::istream& is, Str& s);

#endif
