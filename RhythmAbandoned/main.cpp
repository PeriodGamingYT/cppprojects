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
    vector<int> s(26);
    cout << "\x1b[2J\x1b[0;0f";
    for(char i : r.dt[time]) s[i-'a'] = 1;
    for(int i = time; i < time+80; i++)
        for(int j = 0; j < r.dt[time].length(); j++)
            cout << "\n\x1b[" << i-time << "C" << r.dt[time][j];
	while(playing) {
        if(score < 0) playing = false;
        // Input & Score Check.
        gtime++;
        char ch = getch();
        if(s[ch-'a'] == 1 && gtime < r.bpm) score++;
        else score--;
        if(gtime >= r.bpm) {
            gtime = 0;
            time++;
            for(char i : r.dt[time]) s[i-'a'] = 1;
            cout << "\x1b[2J\x1b[0;0f";
            for(int i = time; i < time+80; i++)
                for(int j = 0; j < r.dt[time].length(); j++)
                    cout << "\n\x1b[" << i-time << "C" << r.dt[time][j];
        }
	}
    return 0;
}
