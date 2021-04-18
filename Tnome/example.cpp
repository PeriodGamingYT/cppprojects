#include <iostream>
#include <vector>
#include <string>
#include "ui.cpp"
using namespace std;

int main() {
	App a;
	a.menu.acts.insert(a.menu.acts.end(), "App Example");
	a.menu.sact = 0;
	BorderLine b;
	b.startx = 2;
	b.starty = 2;
	b.length = 12;
	a.w.insert(a.w.end(), b.update());
	a.print();
}
