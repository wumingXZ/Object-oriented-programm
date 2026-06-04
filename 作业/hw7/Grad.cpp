#include <algorithm>
#include "Grad.h"
#include "grade.h"

using namespace std;

Grad::Grad() : thesis(0) {}

Grad::Grad(istream& in) : thesis(0) {
    read(in);
}

double Grad::grade() const {
    double hw_grade = ::grade(midterm, final, homework);
    return min(thesis, hw_grade);
}

istream& Grad::read(istream& in) {
    read_common(in);
    in >> thesis;
    read_hw(in, homework);
    return in;
}
