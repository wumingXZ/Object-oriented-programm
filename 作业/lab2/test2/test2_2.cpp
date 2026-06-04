#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

std::vector<string> frame(const std::vector<string> & v) {
	int maxlen = 0;
	for (int i = 0; i < v.size(); ++i) {
		if (maxlen < v[i].length()) {
			maxlen = v[i].length();
		}
	}
	
	cout << string(maxlen + 4, '*') << endl;
	for (int i = 0; i < v.size(); ++i) {
		cout << "* ";
		cout << setw(maxlen +1) << left << v[i];	
		cout << "*" << endl;
	}
	cout << string(maxlen + 4, '*') << endl;
	return v;
}

int main() 
{ 
	//TODO
	vector<string> s;
	s.push_back("This is an");
	s.push_back("example");
	s.push_back("to");
	s.push_back("illustrate");
	s.push_back("framing.");
	frame(s);
    
	return 0; 
}
