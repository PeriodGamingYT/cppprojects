#include <string>
#include <iostream>
#include <vector>
#include "border.cpp"
using namespace std;

class Label {
public:
	Border b;
	string message;
	void drawLabel() {
		b.drawBorder();
		cout << "\x1b[" << b.stx+2 << ";" << b.sty+2 << "f";
		vector<string> lines(0);
		int d = b.scx-b.stx-1;
		int r = d%message.length();
		for(int i = 0; i < message.length(); i += d) {
			lines.insert(lines.end(), message.substr(i, i+d));
			lines[lines.size()-1].resize(d);
		}
		for(string i : lines)
			cout << i << "\n\x1b[" << b.stx+1 << "C";
	}
};

Label makeLabelFree(int sx, int sy, int mx, string ms) {
	int scy = ((int)(ms.length()-1)/mx);
	Border a = makeBorder(sx, sy, mx, scy, '=');
	Label b;
	b.b = a;
	b.message = ms;
	return b;
}

Label makeLabel(int sx, int sy, int scx, int scy, string ms) {
	Border a = makeBorder(sx, sy, scx, scy, '=');
	Label b;
	b.b = a;
	b.message = ms;
	return b;
}