#include <iostream>
#include "ui.cpp"
using namespace std;

int main() {
	bool uzing = true;
	Label l = makeLabelFree(0, 0, 5, "Hi!");
	while(uzing) {
		printMouse();
		int c = getch();
		if(c == 'e') uzing = false;
		updMouse(c);
		clear();
		l.drawLabel();
		if(isHover(mousex, mousey, l)) l.message = "!iH";
		else l.message = "Hi!";
	}
}