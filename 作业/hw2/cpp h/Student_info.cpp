#include "Student_info.h"

using namespace std;

//TODO

// struct Student_info{
// 	std::string name;
// 	double midterm, final;
// 	std::vector<double> homework;
// 	bool operator<(const Student_info &n)const{
// 		return name<n.name;
// 	}
// };

std::istream & read_hw(std::istream & in, std::vector<double> & hw){
    if(in) {
        hw.clear();
        double x;
        while(in >> x) {
            hw.push_back(x);
        }
        in.clear();
    }
    return in;
}

std::istream & read(std::istream & is, Student_info &s) {
    is >> s.name >> s.midterm >> s.final;
    read_hw(is, s.homework);
    return is;
}
