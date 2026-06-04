#include <istream>
#include <stdexcept>
#include "grade.h"
#include "Student_info.h"

using namespace std;

Student_info::Student_info() : cp(0) {}

Student_info::Student_info(istream& in) : cp(0) {
    read(in);
}

Student_info::Student_info(const Student_info& s) : cp(0) {
    if (s.cp) cp = s.cp->clone();
}

Student_info::~Student_info() {
    delete cp;
}

Student_info& Student_info::operator=(const Student_info& s) {
    if (this != &s) {
        delete cp;
        cp = 0;
        if (s.cp) cp = s.cp->clone();
    }
    return *this;
}

string Student_info::name() const {
    if (cp) return cp->name();
    throw runtime_error("uninitialized Student_info");
}

double Student_info::grade() const {
    if (cp) return cp->grade();
    throw runtime_error("uninitialized Student_info");
}

istream& Student_info::read(istream& in) {
    delete cp;
    cp = 0;
    char ch;
    in >> ch;
    if (ch == 'U')
        cp = new Core;
    else if (ch == 'G')
        cp = new Grad;

    if (cp)
        cp->read(in);
    return in;
}

bool Student_info::compare_grade(const Student_info& x, const Student_info& y) {
    return x.grade() < y.grade();
}
