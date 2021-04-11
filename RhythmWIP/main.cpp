#include <iostream>
#include <string>
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
        if(score < 0) playing = false;
		string c = "";	

		// Time system.
		gtime++;
        char ch = getch();
		if(ch >= 'a') {
			string o = " ";
			o[0] = ch;
			c.append(o);
		}
		if(gtime > r.bpm) {
			gtime = 0;
			time++;

			// Showing rhythm state.
            string cs = r.dt[time];
            /*for(int i = time; i < time+80; i++) {
				for(int j = 0; j < cs.length(); j++)
					cout << cs[j] << "\n\x1b[" << i << "C";
			}*/

			// Seeing if the user presses the right keys.
			vector<int> a(26);
			vector<int> b(26);
			for(int i = 0; i < cs.length(); i++) {
                a[cs[i]-'a'] = 1;
                b[c[i]-'a'] = 1;
			}

            for(int i = 0; i < 26; i++) {
                if(b[i] != a[i]) score--;
                else score++;
            }
            c = "";
		}
	}
    return 0;
}
