class v2 {
  public:
    int x;
    int y;
  v2 operator()(int X, int Y) {
    v2 a;
    a.x = X;
    a.y = Y;
    return a;
  }

  v2 operator-(v2 a) {
    v2 b;
    b = b(x-a.x, y-a.y);
    return b;
  }

  v2 operator+(v2 a) {
    v2 b;
    b = b(x+a.x, y+a.y);
    return b;
  }

  bool operator==(v2 a) {
    if(x == a.x && y == a.y) return true;
    return false;
  }
};
