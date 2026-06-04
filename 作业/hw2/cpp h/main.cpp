#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <list>
#include "grade.h"
#include "Student_info.h"
using namespace std;

list<Student_info> extract_fails(list<Student_info> & students)
{
	//TODO
	list<Student_info> fail;
	auto iter = students.begin();
	while (iter != students.end()) {
		if (fgrade(*iter)) {
			fail.push_back(*iter);
			iter = students.erase(iter);
		}
		else {
			++iter;
		}
	}
	return fail;
}


int main() 
{ 
	//TODO
	list<Student_info> stu, fail;
	Student_info record;
	int maxlen = 0;
	
	cout << "Please enter students' information:" 
	     << "(Ctrl+Z for Windows, Ctrl+D for Mac/Linux to stop input)"
		 << endl;

	while(read(cin, record)) {
		maxlen = max(maxlen, (int)record.name.size());
		stu.push_back(record);
	}

	stu.sort(); 
	
	cin.clear();
	
	fail = extract_fails(stu);
	auto prec = cout.precision();
	
	cout << "\n=====Passing=====" << endl;
	for (auto i = stu.begin(); i != stu.end(); ++i) {
		cout << setw(maxlen + 1) << left << i -> name << " ";
		try {
			double final = grade(*i);
			cout << setprecision(3) << "Your final grade is" << " "
				 << final << setprecision(prec) << "." << endl;
		} catch (domain_error e) {
			cout << e.what();
		}
		cout << endl;
	}

	cout << "\n=====Failure=====" << endl;
	for (auto i = fail.begin(); i != fail.end(); ++i) {
		cout << setw(maxlen + 1) << left << i -> name << " ";
		try {
			double final = grade(*i);
			cout << setprecision(3) << "Your final grade is" << " "
				 << final << setprecision(prec) << "." << endl;
		} catch (domain_error e) {
			cout << e.what();
		}
		cout << endl;
	}
	return 0; 
}
