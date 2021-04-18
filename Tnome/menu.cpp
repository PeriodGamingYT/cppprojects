#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Menu {
public:
	vector<string> acts;
	int sact;
	void print() {
		cout << "\x1b[0;0f\x1b[K";
		for(int i = 0; i < acts.size(); i++) {
			if(i == sact) cout << "\x1b[7m";
			cout << acts[i] << "\x1b[0m ";
		}
		cout << endl;
	}
};
