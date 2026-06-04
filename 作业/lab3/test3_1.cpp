#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

int main() 
{ 
	//TODO
	std::map<std::string, int> log;
	std::ifstream inf("test3_1_input.txt");
	if (!inf) {
		std::cout << "test3_1_input.txt打开失败" << std::endl;
		return 1;
	}
	std::string word;
	while(inf >> word) {
		log[word]++;
	}
	typedef std::map<std::string, int>::iterator miter;
	for (miter i = log.begin(); i != log.end(); ++i) {
		std::cout << i->first << " " << i->second << std::endl;
	}
    
	return 0; 
}
