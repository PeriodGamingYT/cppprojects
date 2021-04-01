#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "viewer.cpp"
#include "dialog.cpp"
#include "parse.cpp"

#define ctrl(c) ((c) & 0x1f)

using namespace std;

// Don't eat up terminal display data.
void clear() {
	cout << "\x1b[0;0f\x1b[2J\x1b[0;0f";
}

void create(vector<string> lines) {
	for(int i = 0; i < lines.size()-1; i += 2) {
		string name = lines[i];
		Path a;
		vector<string> p(split(lines[i+1]));
		a.symbol = p[0][0];
		a.pchar = p[1][0];
		a.name = name;
		a.r = Stoi(p[2]);
		a.g = Stoi(p[3]);
		a.b = Stoi(p[4]);
		for(int i = 5; i < p.size(); i += 2)
			a.addData(gv2(Stoi(p[i]), Stoi(p[i+1])));
		Objs.insert(Objs.end(), a);
	}
}

int main() {
	bool uzing = true;
	while(uzing) {
		clear();
		makeView();
		chgnav();
		shownav();
		int c = getch();
		if(c == ctrl('s')) uzing = false;
		if(c == 127) {
			string fname = dialog("Please put in a file to read.");
			vector<string> lines(read(fname));
			Objs.resize(0);
			navsc = 0;
			create(lines);
		}
		if(c == '-' || c == '=') {
			navsel += c == '-' ? -1 : 1;
			if(navsel < 0) navsel = 0;
			if(navsel >= items.size()) navsel = items.size()-1;
			if(navsel > 24) navsc = ((int)navsel/24)*24;
		}
	}
}
