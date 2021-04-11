#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> read(string name) {
	vector<string> ans(0);
	fstream file;
	file.open(name, ios::in);
	if(!file) return ans;
    while(!file.eof()) {
        string temp = "";
        getline(file, temp);
		ans.insert(ans.end(), temp);
    }
	return ans;
}

vector<string> split(string s) {
	vector<string> ans(0);
	string token = "";
	for(char i : s) {
		if(i != ' ') {
            string temp = " ";
            temp[0] = i;
			token.append(temp);
		} else {
			ans.insert(ans.end(), token);
			token = "";
		}
	}
	ans.insert(ans.end(), token);
	return ans;
}

int Stoi(string s) {
	int ans = 0;
	int c = 1;
	for(int j = s.length()-1; j >= 0; j--) {
		char i = s[j];
		ans += (i-'0')*c;
		c *= 10;
	}
	return ans;
}
