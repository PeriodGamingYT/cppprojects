#include <iostream>
#include <vector>
#include <cstdlib>
#include "snake.cpp"
#include "getch.h"

using namespace std;

int random(int mn, int mx) {
  return rand() % mx + mn;
}

int main() {
  int speed;
  cout << "Difficulty: ";
  cin >> speed;
  speed = 5-(speed-1);
  Snake s;
  v2 apple;
  apple = apple(random(0, 24), random(0, 80));
  s.init();
  bool playing = true;
  while(playing) {
    // Get input for velocity purposes.
    int c = getch(speed);
    if(c == 'a') s.vel = s.vel(0, -1);
    if(c == 'd') s.vel = s.vel(0, 1);
    if(c == 'w') s.vel = s.vel(-1, 0);
    if(c == 's') s.vel = s.vel(1, 0);
    if(c == 'e') playing = false;
    s.move();
    // See if snake should be dead.
    int head = s.data.size()-1;
    if(s.data[head].x < 1) playing = false;
    if(s.data[head].x > 24) playing = false;
    if(s.data[head].y < 1) playing = false;
    if(s.data[head].y > 80) playing = false;
    for(int i = 0; i < s.data.size()-1; i++)
     if(s.data[i] == s.data[head]) playing = false;

    // Has the snake eaten the apple?
    if(s.data[head] == apple) {
      s.add();
      apple = apple(random(0, 24), random(0, 80));
    }

    // Draw Snake Game.
    cout << "\x1b[2J";
    cout << "\x1b[" << apple.x << ";" << apple.y << "f@";
    for(v2 i : s.data)
      cout << "\x1b[" << i.x << ";" << i.y << "f#";
  }
}
