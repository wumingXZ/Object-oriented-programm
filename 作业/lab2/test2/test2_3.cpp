#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

//TODO
double myop(const double & a, const double & b, const string & op) {
    if (op == "+") 
        return a + b;
    if (op == "-") 
        return a - b;
    if (op == "*") 
        return a * b;
    if (op == "/") {
        if (b == 0) {
            throw runtime_error("除数不能为0");
        }
        return a / b;
    }
    throw "无效操作符";
}

bool isop(const string & op) {
    return (op == "+" || op == "-" || op == "*" || op == "/");
}

void calcu() {
    vector<double> exp;
    string s;

    while(cin >> s) {
        if (!isop(s)) {
            try {
                    exp.push_back(stod(s));
            } 
            catch (const invalid_argument& e) {
                throw("无效数据格式" + s);
                }
        }
        else {
            if (exp.size() < 2) {
                    throw("表达式错误！");
            }

            double right = exp.back();  
            exp.pop_back();
            double left = exp.back();   
            exp.pop_back();
            double ret = myop(left, right, s);
            exp.push_back(ret);
        }
    }
    cout << exp.front() << endl;
    return;
}

int main() {
    std::cout << "Please enter the expression:" << endl;
    std::cout << "(Ctrl+Z for Windows, Ctrl+D for Mac to stop input)" << endl;
    calcu();

    return 0;
}