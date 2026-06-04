#include <iostream>
#include <string>
#include <vector>
using namespace std;

std::vector<string> split(const std::string & s) {
	std::string word = "";
	std::vector<string> ret;
	for (int i = 0; i < s.length(); ++i) {
		if (s[i] != ' ') {
			word.push_back(s[i]);	
		}
		else if (s[i] == ' ' && i < s.length() - 1 && s[i+1] != ' ') {
			ret.push_back(word);
			word.clear();
		}
	}
	ret.push_back(word);
	return ret;
}

int main() 
{ 
	//TODO
	std::vector<string> a = split("I am  from Shanghai.");	
	for (int i = 0; i < a.size(); ++i) {
		cout << "\"" << a[i] << "\"" << endl;
	}
	return 0; 
}
