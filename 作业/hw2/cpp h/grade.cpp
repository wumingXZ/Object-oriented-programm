#include <vector>
#include <stdexcept>
#include "grade.h"
#include "median.h"

using namespace std;

//TODO
double grade(double midterm, double final, double homework) {
    return 0.2 * midterm + 0.4 * final + 0.4 * homework;
}

double grade(double midterm, double final, const std::vector<double> & hw) {
    if (hw.size() == 0) {
        throw domain_error("Student doesn't finish any homework.");
    }
    double fhw = 0.4 * median(hw) + 0.6 * average(hw);
    return grade(midterm, final, fhw);
}

double grade(const Student_info &s) {
    return grade(s.midterm, s.final, s.homework);
}

bool fgrade(const Student_info &s) {
    return grade(s) < 60;
}
