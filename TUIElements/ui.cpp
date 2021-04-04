#include "button.cpp"
#include "mouse.cpp"
#include "getch.h"
#include <iostream>
using namespace std;

void clear() {
	cout << "\x1b[0;0f";
	for(int i = 0; i < 24; i++) {
		for(int j = 0; j < 80; j++) cout << " ";
		cout << endl;
	}
	cout << "\x1b[0;0f";
}