#include "Student_info.h"
#include "grade.h"

using namespace std;

Student_info::Student_info() : midterm(0), final(0) {}

Student_info::Student_info(istream& is) {
    read(is);
}

istream& Student_info::read(istream& in) {
    in >> n >> midterm >> final;
    if (in) {
        homework.clear();
        double x;
        while (in >> x)
            homework.push_back(x);
        in.clear();   
    }
    return in;
}

double Student_info::grade() const {
    return ::grade(midterm, final, homework);
}

string Student_info::name() const {
    return n;
}

bool Student_info::valid() const {
    return !homework.empty();
}

bool compare(const Student_info& x, const Student_info& y) {
    return x.name() < y.name();
}
