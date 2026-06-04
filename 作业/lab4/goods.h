#pragma once
#include<exception>
#include <vector>
#include <string>

class goods
{
    public:
        std::string name;
        int num;
        goods(std::string s, int n) : name(s), num(n) {}
};

class stock
{
    private:
        std::vector<goods> s;
    public:
        void purchase(const goods &g) {
            int i = 0;
            for (; i < s.size(); ++i){
                if (s[i].name == g.name) {
                    s[i].num += g.num;
                    break;
                }
            }
            if (i == s.size()) {
                s.push_back(g);
            }
        }
         
        void sell(const goods &g) {
            int i = 0;
            for(; i < s.size(); ++i) {
                if(s[i].name == g.name) {
                    s[i].num -= g.num;
                    if (s[i].num < 0) {
                        throw std::domain_error("Error: insufficient inventory of pen");
                    }
                    else if (s[i].num == 0) {
                        std::cout << "Warning: Inventory of mouse is 0" << std::endl;
                    }
                }
            }
        }
        
        void print() const {
            sort(s.begin(), s.end(), [] (goods a, goods b) {return a.num > b.num;});
            for (int i = 0; i < s.size(); ++i) {
                std::cout << s[i].name << "\t" << s[i].num << std::endl; 
            }
            std::cout << std::endl;
        }
};