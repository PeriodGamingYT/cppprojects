#include <iostream>
#include <vector>
#include "prop.cpp"

using namespace std;

// Line algorithm.
vector<v2> mkline(v2 a, v2 b) {
	vector<v2> ans(0);
	int mnew = 2 * (b.y-a.y);
	int sloperr = mnew - (b.x-a.x);
	for(int x = a.x, y = a.y; x < b.x; x++) {
		ans.resize(ans.size()+1);
		ans[ans.size()-1] = gv2(x, y);
		sloperr += mnew;
		if(sloperr >= 0) {
			y++;
			sloperr -= 2 * (b.x-a.x);
		}
	}
	return ans;
}

// Show picture in viewer.
void makeView() {
	cout << "\x1b[0;0f";
	for(int i = Objs.size()-1; i >= 0; i--) {
		for(int k = 0; k < Objs[i].data.size()-1; k++) {
			vector<v2> line(mkline(Objs[i].data[k], Objs[i].data[k+1]));
			for(v2 j : line) {
				cout << "\x1b[" << j.x << ";" << j.y << "f";
				cout << "\x1b[38;2;" << Objs[i].r << ";" << Objs[i].g << ";" << Objs[i].g << "m";
				cout << Objs[i].pchar << "\x1b[0m";
			}
		}
	}
	cout << "\x1b[0;0f";
}
