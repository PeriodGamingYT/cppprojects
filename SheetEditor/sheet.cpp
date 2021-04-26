#include "cell.cpp"
#include "ansi.cpp"
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>

using namespace std;

class Sheet {
public:
    pair<int, int> defcs;
    pair<int, int> vscells;
    pair<int, int> scroll;
    pair<int, int> cursor;
    vector<pair<int, int>> sel;
    map<pair<int, int>, Cell> cells;
    pair<int, int> getcsize(pair<int, int> cords) {
        pair<int, int> ans = cs[cords];
        if(!ans.first) ans.first = defcs.first;
        if(!ans.second) ans.second = defcs.second;
        ans.first++;
        ans.second++;
        return ans;
    }

    void setcssize(pair<int, int> cords, pair<int, int> value) {
        cs[cords] = value;
    }

    void printsheet() {
        for(int y = scroll.second; y < vscells.second+scroll.second; y++) {
            for(int x = scroll.first; x < vscells.first+scroll.first; x++) {
                bool selected = false;
                for(int i = 0; i < sel.size(); i++)
                    if(sel[i].first == x && sel[i].second == y)
                        selected = true;
                if(cursor.first == x && cursor.second == y)
                    selected = true;
                pair<int, int> size(getcsize(make_pair(x, y)));
                string cellstr = cells[make_pair(x, y)].outdt;
                int stringi = 0;
                int Y = (y-scroll.second)*size.second;
                int X = (x-scroll.first)*size.first;
                for(int sy = Y; sy < Y+size.second; sy++)
                    for(int sx = X; sx < X+size.first; sx++) {
                        setpos(sx, sy);
                        if(sx == X || sx == X+size.first || 
                        sy == Y || sy == Y+size.second)
                            cout << "=";
                        else {
                            if(selected) contrast();
                            if(stringi < cellstr.length()) {
                                cout << cellstr[stringi];
                                stringi++;
                            } else cout << " ";
                            normal();
                        }
                    }
            }
        }
    }
private:
    map<pair<int, int>, pair<int, int>> cs;
};

// Fine tuned to fit 80x24.
const int vscellsx = 9;
const int vscellsy = 12;