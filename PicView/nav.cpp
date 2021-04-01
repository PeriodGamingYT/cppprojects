#include <iostream>
#include <string>
#include <vector>
#include "path.cpp"
using namespace std;

// Paring functions.
string Itos(int n) {
 if(n == 0) return "0";
 int a = n;
 string s;
 while(a > 0) {
    string t = " ";
    t[0] = (a%10)+'0';
    s.append(t);
    a /= 10;
 }
 for (int i = 0; i < s.length() / 2; i++)
        swap(s[i], s[s.length() - i - 1]);
 return s;
}

// Navagator.
int navsc = 0;
int navsel = 0;
vector<Path> Objs(0);
vector<string> items(0);

// Navagation UI.
void chgnav() {
	items.resize(0);
	for(Path i : Objs) {
		string cItem = "   ";
		cItem[2] = i.symbol;
		cItem.append(" ");
		cItem.append(i.name);
		items.resize(items.size()+1);
		items[items.size()-1] = cItem;
		for(v2 j : i.data) {
			cItem = "     ";
			cItem.append(Itos(j.x));
			cItem.append(", ");
			cItem.append(Itos(j.y));
			cItem.append(".");
			items.resize(items.size()+1);
			items[items.size()-1] = cItem;
		}
	}
}

void shownav() {
	for(int i = 0; i < 24; i++)  {
		cout << "\x1b[55C=";
		if(i+navsc < items.size()) {
			if(i+navsc == navsel) cout << "\x1b[7m";
			cout << items[navsc+i] << "\x1b[0m";
		}
		cout << endl;
	}
}
