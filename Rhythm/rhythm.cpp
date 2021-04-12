#include <map>
#include <string>
#include <vector>
#include "parse.cpp"
using namespace std;

// Rhythm.
class Rhythm {
public:
  map<int, string> dt;
  int speed, end;
};

Rhythm makeRhythm(vector<string> lines) {
  Rhythm ans;
  ans.speed = Stoi(split(lines[0])[0]);
  ans.end = Stoi(split(lines[0])[1]);
  for(int i = 1; i < lines.size(); i++)
    ans.dt[Stoi(split(lines[i])[0])] = split(lines[i])[1];
  return ans;
}