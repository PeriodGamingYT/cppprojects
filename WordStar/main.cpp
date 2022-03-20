#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "getch.h"

#define ctrl(c) ((c) & 0x1f)

using namespace std;

void clear() {
	cout << "\x1b[0;0f";
	cout << "\x1b[2J";
	cout << "\x1b[0;0f";
}

string showPrompt(string prompt) {
	clear();
	cout << prompt;
	string result = "";
	getline(cin, result);
	clear();
	return result;
}

void saveFile(string fileName, vector<string> lines) {
	std::ofstream file(fileName, std::ofstream::trunc);
	if(!file) {
		cout << "Error creating file.\n";
		return;
	}

	for(string i : lines) {
		file << i << endl;
	}

	file.close();
}

int main(char **argv, int argc) {
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
			string fileName = showPrompt("Save file with name: ");
			saveFile(fileName, lines);
			return 0;
		} else if(c == ctrl('q')) {
			string answer = showPrompt("If you want to quit without saving type \"Yes do as I say!!!\": ");
			if(answer == "Yes do as I say!!!") {
				return 0;
			}
		} else {
			string s = " ";
			s[0] = c;
			lines[srow].append(s);
			scol++;
		}
	}
}
