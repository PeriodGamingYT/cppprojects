#include "getch.h"
#include <iostream>
#include <string>

using namespace std;

string dialog(string prompt) {
	cout << "\x1b[0;0f";
	string ans;
	cout << prompt << endl;
	cin >> ans;
	return ans;
}
