#include "widgets.cpp"
#include "menu.cpp"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class App {
public:
	Menu menu;
	vector<WidgetC> w;
	void print() {
		cout << "\x1b[2J\x1b[0;0f";
		menu.print();
		for(WidgetC i : w) i.print();
	}
};
