// Public File so that mouse pos is correct.
int mousex = 0;
int mousey = 0;
bool mouseDown = false;

#include <iostream>
using namespace std;

void printMouse() {
	cout << "\x1b[" << mousey << ";" << mousex << "f#";
}

void updMouse(char c) {
	if(c == 'w') mousey--;
	if(c == 's') mousey++;
	if(c == 'a') mousex--;
	if(c == 'd') mousex++;
	if(mousex < 0) mousex = 0;
	if(mousey < 0) mousey = 0;
	if(mousex > 80) mousex = 80;
	if(mousey > 24) mousey = 24;
	mouseDown = (c == 'q');
}