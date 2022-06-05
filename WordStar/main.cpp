#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
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
	ofstream file(fileName, std::ofstream::trunc);
	if(!file) {
		cout << "Error creating file.\n";
		return;
	}

	for(string i : lines) {
		file << i << endl;
	}

	file.close();
}

vector<string> openFile(string fileName) {
  vector<string> result(1);
  result[0] = "";
  ifstream file(fileName);
  if(file) {
    result.pop_back();
    string strip;
    while(!file.eof()) {
      getline(file, strip);
      result.push_back(strip);
    }
  }
  file.close();
  return result;
}

int main(int argc, char **argv) {
	// Document data.
	vector<string> lines;
	int srow = 0;
	int scol = 0;
	if(argc == 2) {
	  cout << argc << " " << argv[0] << endl;
	  lines = openFile(argv[1]);
	  srow = max(0, (int)(lines.size() - 1));
	  scol = max(0, (int)(lines[srow].size() - 1));
	} else {
	  lines = vector<string>(1);
 	  lines[0] = "";
	}
	// Editing.
	while(1) {
		// Show editor output.
		clear();
		for(int i = 0; i < (int)lines.size(); i++)
		  cout << lines[i] << (i >= (int)lines.size() - 1 ? "" : "\n");
		// Handling Input.
		// « used to be cursor.
		int c = getch();
		if(c == '\r') { // Enter ASCII = '\r'.
			scol = 0;
			srow++;
			lines.push_back("");
		} else if(c == 127) { // Backspace ASCII = 127.
			if(lines[srow].size() <= 0) {
				scol = 0;
				if(lines.size() > 1) {
				  srow--;
					lines.erase(lines.end());
					srow = lines.size() - 1;
				} else {
					srow = 0;
					scol = 0;
				}
			} else {
			  scol--;
				lines[srow].erase(lines[srow].size() - 1);
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
