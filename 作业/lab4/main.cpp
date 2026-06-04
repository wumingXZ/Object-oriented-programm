#include <iostream>
#include "goods.h"
//TODO
using namespace std;
//TODO

int main()
{
    stock s;
    s.purchase(goods("pen", 100));
    s.purchase(goods("mouse", 50));
    s.purchase(goods("notepad", 200));
    s.purchase(goods("pen", 50));
    s.print();

    try{
        s.sell(goods("mouse", 50));
    }catch (domain_error e) {
        cout << e.what() << endl;
    }
    try{
        s.sell(goods("pen", 210));
    }catch (domain_error e) {
        cout << e.what() << endl;
    }

    return 0;
}
