#include <iostream>
using namespace std;
class Border {
public:
	int stx;
	int sty;
	int scx;
	int scy;
	char blook;
	void drawBorder() {
		cout << "\x1b[" << stx << ";" << sty << "f";
		for(int y = stx; y <= scy; y++) {
			for(int x = stx; x <= scx; x++) {
				if(x == stx || x == scx || y == sty || y == scy)
				 cout << blook;
				else
				 cout << " ";
			}
			if(stx > 0)
			 cout << "\n\x1b[" << stx << "C";
			else
			 cout << "\n";
		}
	}
};

Border makeBorder(int sx, int sy, int scx, int scy, char sym) {
	Border a;
	a.stx = sx;
	a.sty = sy;
	a.scx = scx+sx+1;
	a.scy = scy+sy+2;
	a.blook = sym;
	return a;
}
