#include "algorithm.h"
#include "Student_info.h"

using std::istream;
using std::vector;
using std::cin;
using MySpace::find;

istream & read_hw(istream & in, vector<double> & hw)
{
	if(in){
		hw.clear();
		double x;//a variable into which to read
		while(in >> x){
			hw.push_back(x);
		}
		in.clear();
	}
	return in;
}
istream & read(istream & is, Student_info &s)
{
	is >> s.name >>s.midterm >>s.final;
	read_hw(is, s.homework);
	return is;
}

bool did_all_hw(const Student_info & s)
{
	return((MySpace::find(s.homework.begin(), s.homework.end(), 0)) 
			== s.homework.end());
}
