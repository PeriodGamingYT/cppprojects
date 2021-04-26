#include "editor.cpp"
#include <iostream>
#include <utility>
#include <sstream>
#include "getch.h"

using namespace std;

void gtstatus() {
    setpos(0, 24);
    cout << "\x1b[K";
}

#define TOSTR(s) #s

int min(int a, int b) {return b < a ? a : b;}
int max(int a, int b) {return b > a ? b : a;}
int mn(int a, int b) {return b < a ? b : a;}

int main() {
    Sheet s;
    s.vscells = make_pair(vscellsx, vscellsy);
    s.cursor = make_pair(0, 0);
    s.scroll = make_pair(0, 0);
    s.defcs = make_pair(8, 1);

    bool uzing = true;
    while(uzing) {
        s.printsheet();
        char c = getch();
        pair<int, int> sc(min(0, s.scroll.first-s.vscells.first),
                          min(0, s.scroll.second-s.vscells.second));
        switch(c) {
            case 'x':
                uzing = false;
                return 0;
                break;
            case 'z':
                s.sel.resize(0);
                break;
            case 'w':
                s.cursor.second--;
                break;
            case 's':
                s.cursor.second++;
                break;
            case 'a':
                s.cursor.first--;
                break;
            case 'd':
                s.cursor.first++; 
                break;
        }

        // If statements because switch() was giving me trouble.
        if(c == 'q') {
            int added = -1;
            for(int i = 0; i < s.sel.size(); i++)
                if(s.sel[i].first == s.cursor.first && 
                    s.sel[i].second == s.cursor.second)
                    added = i;
            if(added == -1)
                s.sel.insert(s.sel.end(), s.cursor);
            else
                s.sel.erase(s.sel.begin()+added);
        } else if(c == 'e') {
            gtstatus();
            string dt;
            cin >> dt;
            for(pair<int, int> i : s.sel) {
                s.cells[i].indt = dt;
                s.cells[i].outdt = format(dt, s.cells[i].format);
            }
        } else if(c == 'f') {
            gtstatus();
            string fm;
            cin >> fm;
            for(pair<int, int> i : s.sel) {
                s.cells[i].format = fm;
                s.cells[i].outdt = format(s.cells[i].indt, fm);
            }
        } else if(c == 'c') {
            pair<int, int> st(mn(s.sel[0].first, s.cursor.first),
                              mn(s.sel[0].second, s.cursor.second));
            pair<int, int> en(max(s.sel[0].first, s.cursor.first),
                              max(s.sel[0].second, s.cursor.second));
            s.sel.resize(0);
            for(int y = st.second; y <= en.second; y++)
                for(int x = st.first; x <= en.first; x++)
                    s.sel.insert(s.sel.end(), make_pair(x, y));
        } else if(c == 'r') {
            gtstatus();
            string formula;
            cin >> formula;
            float ans = solve(formatToExp(formula, s));
            stringstream str;
            str << ans;
            string dt = str.str();
            for(pair<int, int> i : s.sel) {
                s.cells[i].indt = dt;
                s.cells[i].outdt = format(dt, s.cells[i].format);
            }
        }
        s.cursor = make_pair(min(0, s.cursor.first), 
                             min(0, s.cursor.second));
        if(s.cursor.first < sc.first) s.scroll.first--;
        if(s.cursor.first > sc.first+s.vscells.first) s.scroll.first++;
        if(s.cursor.second < sc.second) s.scroll.second--;
        if(s.cursor.second > sc.second+s.vscells.second) s.scroll.second++;
    }
}