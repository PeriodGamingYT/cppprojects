#include <map>
#include <string>
#include <vector>
#include "file.cpp"

using namespace std;

class Rhythm {
public:
	map<string, int> dt;
	int bpm;
};

Rhythm makeRhythm(vector<string> s) {
	Rhythm a;
	a.bpm = stoi(split(s[0])[0]);
	for(int i = 1; i < s.size(); i++) {
		vector<string> cline(split(s[i]));
		dt[stoi(cline[0])] = cline[1];
	}
	return a;
}