#include <vector>
#include <stdexcept>
#include "grade.h"
#include "median.h"

using namespace std;

double grade(double midterm, double final, const vector<double>& hw) {
    if (hw.empty())
        throw domain_error("student has done no homework");
    return 0.2 * midterm + 0.4 * final + 0.4 * median(hw);
}
