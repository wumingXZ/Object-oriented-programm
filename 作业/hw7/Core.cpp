#include "Core.h"
#include "grade.h"

using namespace std;

Core::Core() : midterm(0), final(0) {}

Core::Core(istream& in) : midterm(0), final(0) {
    read(in);
}

string Core::name() const { return n; }

double Core::grade() const {
    return ::grade(midterm, final, homework);
}

istream& Core::read_common(istream& in) {
    in >> n >> midterm >> final;
    return in;
}

istream& Core::read(istream& in) {
    read_common(in);
    read_hw(in, homework);
    return in;
}

istream& read_hw(istream& in, Vec<double>& hw) {
    if (in) {
        hw.clear();
        double x;
        while (in >> x)
            hw.push_back(x);
        in.clear();
    }
    return in;
}

bool compare(const Core& x, const Core& y) {
    return x.name() < y.name();
}

bool compare_grade(const Core& x, const Core& y) {
    return x.grade() < y.grade();
}

bool compare_grade_ptr(const Core* x, const Core* y) {
    return compare_grade(*x, *y);
}
