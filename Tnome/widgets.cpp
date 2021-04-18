#include <string>
#include <vector>
#include <iostream>
#include <map>

using namespace std;

// Widget Container.
class WidgetC {
public:
	vector<string> mtx;
	int startx;
	int starty;
	void print() {
		for(int y = 0; y < mtx.size(); y++)
			cout << "\x1b[" << starty+y+1 << ";" << startx << "f" << mtx[y];
	}
	
	void set(int i, string s) {
		if(i >= mtx.size()) mtx.resize(i+1);
		mtx[i].resize(s.length());
		mtx[i] = s;
	}
};

// Some premade widgets.
class Border {
public:
	int startx;
	int starty;
	int endx;
	int endy;
	WidgetC update() {
		WidgetC wc;
		wc.startx = startx;
		wc.starty = starty;
		for(int y = starty; y <= endy+3; y++) {
			string s = "";
			for(int x = startx; x <= endx+3; x++)
				if(y == starty || x == startx || x == endx+3 || y == endy+3)
					s.append("=");
				else
					s.append(" ");
			wc.set(y-starty, s);
		}
		return wc;
	}
};

class BorderLine {
public:
	int startx;
	int starty;
	int vert;
	int length;
	WidgetC update() {
		WidgetC wc;
		wc.startx = startx;
		wc.starty = starty;
		if(vert==0) {
			for(int i = 0; i < length; i++)
				wc.set(i, "=");
		} else {
			string s = "";
			for(int i = 0; i < length; i++)
				s.append("=");
			wc.set(0, s);
		}
		return wc;
	}
};
