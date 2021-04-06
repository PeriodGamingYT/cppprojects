#include <vector>
#include "v2.cpp"

using namespace std;

class Snake {
public:
  vector<v2> data;
  v2 vel;

  void move() {
    data.insert(data.end(), data[data.size()-1]+vel);
    data.erase(data.begin());
  }

  void add() {
    data.insert(data.begin(), data[0]-vel);
  }

  void init() {
    v2 a;
    data.insert(data.end(), a(20, 11));
    data.insert(data.end(), a(20, 12));
    vel = a(1, 0);
  }
};
