#include <map>
#include <string>
#include <vector>
#include <utility>
#include <fstream>

using namespace std;

// File Functions.
vector<string> read(string name) {
        vector<string> ans(0);
        fstream file;
        file.open(name, ios::in);
        if(!file) return ans;
        while(!file.eof()) {
                ans.resize(ans.size()+1);
                getline(file, ans[ans.size()-1]);
        }
	file.close();
        return ans;
}

// Parsing Functions.
vector<string> split(string s) {
	vector<string> ans(0);
	string token = "";
	for(char i : s) {
		if(i != ' ') {
			token.resize(token.length()+1);
			token[token.length()-1] = i;
		} else {
			ans.resize(ans.size()+1);
			ans[ans.size()-1] = token;
			token = "";
		}
	}
	ans.resize(ans.size()+1);
	ans[ans.size()-1] = token;
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