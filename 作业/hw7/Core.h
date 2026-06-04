#ifndef GUARD_CORE
#define GUARD_CORE

#include <string>
#include <istream>
#include <iostream>
#include "Vec.h"


class Core{
public:
    Core();
    Core(std::istream&);
    virtual ~Core() {}

    std::string name() const;
    virtual double grade() const;
    virtual std::istream& read(std::istream&);
    virtual Core* clone() const { return new Core(*this); }

protected:
    std::istream& read_common(std::istream&);
    std::string n;
    double midterm, final;
    Vec<double> homework;
};

bool compare(const Core &x, const Core &y);
bool compare_grade(const Core &x, const Core &y);
bool compare_grade_ptr(const Core *x, const Core *y);
std::istream & read_hw(std::istream & in, Vec<double> & hw);

#endif
