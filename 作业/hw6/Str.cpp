#include "Str.h"

Str::Str(const char* cp) {
    while (*cp) {
        data.push_back(*cp);
        ++cp;
    }
}

Str& Str::operator+=(const Str& rhs) {
    for (size_type i = 0; i != rhs.size(); ++i)
        data.push_back(rhs[i]);
    return *this;
}

Str& Str::operator+=(const char* rhs) {
    for (size_type i = 0; rhs[i] != '\0'; ++i)
        data.push_back(rhs[i]);
    return *this;
}

Str& Str::operator+=(char c) {
    data.push_back(c);
    return *this;
}

Str operator+(const Str& lhs, const Str& rhs) {
    Str ret = lhs;
    ret += rhs;
    return ret;
}

Str operator+(const Str& lhs, const char* rhs) {
    Str ret = lhs;
    ret += rhs;
    return ret;
}

Str operator+(const char* lhs, const Str& rhs) {
    Str ret(lhs);
    ret += rhs;
    return ret;
}

std::ostream& operator<<(std::ostream& os, const Str& s) {
    for (Str::size_type i = 0; i != s.size(); ++i)
        os << s[i];
    return os;
}

std::istream& operator>>(std::istream& is, Str& s) {
    s = Str();
    char c;
    while (is.get(c) && isspace(static_cast<unsigned char>(c)))
        ;
    if (is) {
        do {
            s += c;
        } while (is.get(c) && !isspace(static_cast<unsigned char>(c)));
        if (is)
            is.unget();
    }
    return is;
}
