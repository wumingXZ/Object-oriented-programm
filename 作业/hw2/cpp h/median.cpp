#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

//TODO
double median(std::vector<double> vec) {
    auto size = vec.size();
    if (vec.empty()) {
        throw domain_error("Empty homework");
    }
    sort(vec.begin(), vec.end());
    return size % 2 == 0 ? (vec[size / 2 - 1] + vec[size / 2]) / 2 : vec[size / 2];
}

double average(const std::vector<double> &vec) {
    auto size = vec.size();
    if (vec.empty()) {
        throw domain_error("Empty homework");
    }
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += vec[i];
    }    
    sum = sum / size;
    return sum;
}
