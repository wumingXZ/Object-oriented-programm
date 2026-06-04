#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

double median(vector<double> vec) {
    if (vec.empty())
        throw domain_error("median of an empty vector");
    
    sort(vec.begin(), vec.end());
    vector<double>::size_type mid = vec.size() / 2;
    return vec.size() % 2 == 0 ? (vec[mid - 1] + vec[mid]) / 2 : vec[mid];
}
