#include <iostream>
#include <string>
#include <vector>
#include "getch.h"
#include "rhythm.cpp"

using namespace std;

int main() {
  string name;
  cin >> name;
  Rhythm r = makeRhythm(read(name));
  int time = 0;
  int score = 0;
  init();
  cout << "Loaded\n";
  while(score >= 0 && time < r.end) {
    // Output.
    cout << "\x1b[2J\x1b[1;1fScore: " << score;
    for(int i = time; i < time+80; i++) {
      for(int j = 0; j < r.dt[i].length(); j++)
        cout << "\x1b[" << j+2 << ";" << i-time+1 << "f" << r.dt[i][j] << endl;
    }
    cout << "\x1b[50;0f";

    // See if user is pressing the correct keys.
    vector<int> l(26);
    for(int i = 0; i < r.dt[time].length(); i++) l[r.dt[time][i]-'a'] = 1;
    for(int i = 0; i < r.speed; i++) {
      int ch = getch();
      if(ch >= 'a')
       if(l[ch-'a'] == 1 || l[ch-'a'] == -1) {
        l[ch-'a'] = -1;
       } else l[ch-'a'] = 2;
     	if(ch >= 'a') cout << ch << " ";
    }
    for(int i = 0; i < 26; i++) score -= l[i] == 2 ? 1 : l[i];
    cout << endl;
    // Update time.
    time++;
  }
  reset();

  return 0;
}
