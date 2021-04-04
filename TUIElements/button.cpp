#include "label.cpp"
#include <iostream>

using namespace std;

bool isHover(int x, int y, Label l) {
	Border a = l.b;
	return !(x < a.stx || x > a.scx+1 || y < a.sty || y > a.scy+1);
} 