#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include <iterator>


struct Student {
    std::string name;
    int score;
};

void task1() {
    // TODO
    std::vector<int> numbers = {3, 5, 2, 8, 1, 10}; 
    std::vector<int> doubled;
    std::transform (numbers.begin(), numbers.end(),
                   std::back_inserter(doubled),
                   [](int x) { return 2 * x; } );
    typedef std::vector<int>::iterator vi;
    std::cout << "Doubled vector: ";
    for(vi i = doubled.begin(); i != doubled.end(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
    
    std::vector<int> filtered;
    std::copy_if (numbers.begin(), numbers.end(),
                   std::back_inserter(filtered),
                   [](int x) { return x > 5; } );
    std::cout << "Filtered vector (> 5): ";
    for(vi i = filtered.begin(); i != filtered.end(); ++i) {
        std::cout << *i << " ";
    } 
    std::cout << std::endl;

    std::sort(numbers.begin(), numbers.end(), 
              [](int a, int b) {
                return std::abs(a - 5) < std::abs(b - 5);
              });
    std::cout << "Sorted by absolute difference from 5: ";
    for(vi i = numbers.begin(); i != numbers.end(); ++i) {
        std::cout << *i << " ";
    }
    std::cout << std::endl;
}

void task2() {
    // TODO
    std::vector<Student> students = {{"Alice", 85}, {"Bob", 72}, {"Charlie", 90}};
    std::sort(students.begin(), students.end(),
              [](const Student &a, const Student &b) {
                return a.score > b.score;
              });
    
    std::cout << "Students sorted by score (descending):" << std::endl;
    typedef std::vector<Student>::iterator vi;
    for(vi i = students.begin(); i != students.end(); ++i) {
        std::cout << i->name << ": " << i->score << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << "Students with score > 80:" << std::endl;
    std::for_each(students.begin(), students.end(),
                    [](const Student &a) {
                        if (a.score > 80) {
                            std::cout << a.name << std::endl;
                        }
                    });
}

void task3() {
    // TODO
    std::vector<int> numbers = {3, 5, 2, 8, 1, 10};
    int pdt = std::accumulate(numbers.begin(), numbers.end(), 1,
                                [](int acc, int x) {
                                    return (x % 2 == 0) ? acc * x : acc;
                                });
    std::cout << "Product of even numbers: " << pdt << std::endl;
}

int main() {
    std::cout << "Task 1 Solution:" << std::endl;
    task1();

    std::cout << "\nTask 2 Solution:" << std::endl;
    task2();

    std::cout << "\nTask 3 Solution:" << std::endl;
    task3();

    return 0;
}