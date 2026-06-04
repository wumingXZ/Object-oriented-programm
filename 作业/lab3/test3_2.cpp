#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>

//TODO

int main() 
{ 
	//TODO
	std::ifstream inf("test3_2_input.txt");
	if(! inf) {
		std::cout << "文件打开失败" << std::endl;
		return 1;
	}

	std::map<std::string, std::set<int>> words;
	std::string sentence;
	int num = 0;
	while(std::getline(inf, sentence)){
		++num;
		std::istringstream iss(sentence);
		std::string word;
		while(iss >> word) {
			words[word].insert(num);
		}
	}

	typedef std::map<std::string, std::set<int>>::iterator miter;
	typedef std::set<int>::iterator siter;
	for(miter i = words.begin(); i != words.end(); ++i) {
		std::cout << i->first;
		for(siter j = i->second.begin(); j != i->second.end(); ++j) {
			std::cout << " " << *j;
		}
		std::cout << std::endl;
	}

	return 0; 
}
