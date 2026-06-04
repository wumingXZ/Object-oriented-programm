#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "grade.h"
#include "Student_info.h"

using namespace std;

int main() {
    ifstream infile("hw4_input.txt");
    if (!infile) {
        cerr << "无法打开 hw4_input.txt" << endl;
        return 1;
    }
    
    vector<Student_info> students;
    Student_info record;
    
    while (record.read(infile)) {
        if (record.valid())
            students.push_back(record);
    }
    
    sort(students.begin(), students.end(), compare);
    
    for (vector<Student_info>::size_type i = 0; i != students.size(); ++i) {
        cout << students[i].name() << "'s final grade is "
             << students[i].grade() << endl;
    }
    
    return 0;
}
