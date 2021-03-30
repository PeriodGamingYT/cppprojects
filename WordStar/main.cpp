#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "getch.h"

#define ctrl(c) ((c) & 0x1f)

using namespace std;

void clear() {
	cout << "\x1b[0;0f";
	for(int i = 0; i <  24; i++) {
		for(int j = 0; j < 80; j++) cout << " ";
		cout << "\n";
	}
	cout << "\x1b[0;0f";
}

int main() {
	if(argc == 1) return -1;
	// Document data.
	vector<string> lines(1);
	int srow = 0;
	int scol = 0;

	// Editing.
	while(1) {
		// Show editor output.
		clear();
		for(string i : lines) cout << i << endl;
		// Handling Input.
		int c = getch();
		if(c == '\r') {
			scol = 0;
			srow++;
			lines.resize(lines.size()+1);
		} else if(c == 127) {
			if(scol-1 < 0) {
				scol = 0;
				if(srow > 0) {
					srow--;
					lines.erase(lines.begin()+srow+1);
				} else {
					srow = 0;
					scol = 0;
				}
			} else {
				string s = "";
				for(int i = 0; i < lines[srow].length(); i++)
					if(i != scol-1) {
						string o = " ";
						o[0] = lines[srow][i];
						s.append(o);
					}
				lines[srow] = s;
				scol--;
			}
		} else if(c == ctrl('s')) {
			write(argv[1], lines);
			return 0;
		} else {
			string s = " ";
			s[0] = c;
			lines[srow].append(s);
			scol++;
		}
	}
}
