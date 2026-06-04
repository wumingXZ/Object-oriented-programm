#ifndef GUARD_Student_info
#define GUARD_Student_info

#include "Vec.h"
#include <string>
#include <istream>
#include <iostream>
#include "Core.h"
#include "Grad.h"

class Student_info{
public:
    Student_info();
    Student_info(std::istream&);
    Student_info(const Student_info&);
    ~Student_info();
    Student_info& operator=(const Student_info&);

    std::string name() const;
    double grade() const;
    std::istream& read(std::istream&);

    static bool compare_grade(const Student_info& x, const Student_info& y);

private:
    Core* cp;
};

#endif
