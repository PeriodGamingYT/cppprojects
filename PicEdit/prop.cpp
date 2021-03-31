#include <iostream>
#include <string>
#include <vector>
#include "nav.cpp"
using namespace std;

// Properties.
int propSel = 0;
void showProp() {
	cout << "\x1b[55C";
	for(int i = 0; i < 25; i++) cout << "=";
	cout << endl;
	cout << "\x1b[55C= ";
	if(propSel == 0) cout << "\x1b[7m";
	string s = "";
	int pi = -1;
	int vi = -1;
	int sum = 0;
	for(int i = 0; i < Objs.size(); i++) {
		if(navsel == i) {
			pi = 0;
			vi = -1;
		}
		sum++;
		for(int j = 0; j < Objs[i].data.size(); j++) {
			if(j+sum == navsel) {
				pi = i;
				vi = j;
			}
			sum++;
		}
	}
	if(vi == -1 && pi != -1) s = Objs[pi].name;
	else if(vi != -1 && pi != -1){
		s.append(Itos(Objs[pi].data[vi].x));
		s.append(", ");
		s.append(Itos(Objs[pi].data[vi].y));
	}
	cout << s << "\x1b[0m\n\x1b[55C= ";
	if(vi != -1 && pi != 1) {
		if(propSel == 1) cout << "\x1b[7m";
		string s = "X: ";
		s.append(Itos(Objs[pi].data[vi].x));
		cout << s << "\x1b[0m  ";
		if(propSel == 2) cout << "\x1b[7m";
                s = "Y: ";
                s.append(Itos(Objs[pi].data[vi].y));
                cout << s << "\x1b[0m\n\x1b[55C= ";
	} else if(vi == -1 && pi != -1) {
		if(propSel == 1) cout << "\x1b[7m";
		s = "";
		s.append(Itos(Objs[pi].r));
		s.append(", ");
		s.append(Itos(Objs[pi].g));
                s.append(", ");
		s.append(Itos(Objs[pi].b));
		cout << s << "\x1b[0m\n\x1b[55C= ";
		if(propSel == 2) cout << "\x1b[7m";
		cout << Objs[pi].pchar << "\x1b[0m\n";
	} else cout << "\x1b[0m\n\x1b[55C=\n";
	cout << "\x1b[55C=\n\x1b[55C";
	for(int i = 0; i < 25; i++) cout << "=";
	cout << endl;
}
