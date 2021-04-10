#include <iostream>
#include "rhythm.cpp"
#include "getch.h"
using namespace std;

int main() {
	string name = "test.song";
	Rhythm r = makeRhythm(read(name));
	int gtime = 0;
	bool playing = true;
	int score = 0;
	int time = 0;

	while(playing) {
		string c = "";	

		// Time system.
		gtime++;
		if(c >= 'a') {
			string o = " ";
			o[0] = getch();
			c.append(o);
		}
		if(time > bpm) {
			gtime = 0;
			time++;

			// Showing rhythm state.
			for(int i = time; i < time+80) {
				string cstate = dt[time];
				for(int j = 0; j < cstate.length(); j++) 
					cout << cstate[j] << "\n\x1b[" << j << "C";
			}

			// Seeing if the user presses the right keys.
			string cs = dt[time];
			vector<int> a(26);
			vector<int> b(26);
			for(int i = 0; i < cs.length(); i++) {
				for(int i = 0; i < cs.size(); i++) {
					a[cs[i]-'a']++;
					b[c[i]-'a']++;
				}
			}


		}
	}
}