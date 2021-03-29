#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <fstream>

using namespace std;

// File Functions.
vector<string> read(string name) {
        vector<string> ans(0);
        fstream file;
        file.open(name, ios::in);
        if(!file) return ans;
        while(!file.eof()) {
                ans.resize(ans.size()+1);
                getline(file, ans[ans.size()-1]);
        }
	file.close();
        return ans;
}

void write(string name, string s) {
	fstream file;
	file.open(name, ios::out);
	file << s;
	file.close();
} 

// Parsing Functions.
vector<string> split(string s) {
	vector<string> ans(0);
	string token = "";
	for(char i : s) {
		if(i != ' ') {
			token.resize(token.length()+1);
			token[token.length()-1] = i;
		} else {
			ans.resize(ans.size()+1);
			ans[ans.size()-1] = token;
			token = "";
		}
	}
	ans.resize(ans.size()+1);
	ans[ans.size()-1] = token;
	return ans;
}

int Stoi(string s) {
	int ans = 0;
	int c = 1;
	for(int j = s.length()-1; j >= 0; j--) {
		char i = s[j];
		ans += (i-'0')*c; 
		c *= 10;
	}
	return ans;
}

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

// Spread Sheet data.
class Sheet {
	public:
		int scx; // Scrolling offset.
		int scy;
		int selx; // Selection cords.
		int sely;
		vector<pair<int, int> > cc; // "cc" meaning Cell Cords.
		string getData(int x, int y) { // pairs<int, int> because only 1 key is allowed.
			pair<int, int> p(x, y);
			return data[p];
		}
		
		void setData(int x, int y, string e) {
			pair<int, int> p(x, y);
			data[p] = e;
			cc.resize(cc.size()+1);
			cc[cc.size()-1] = p;
		}

        private:
                map<pair<int, int>, string> data;
};

Sheet createsheet(vector<string> lines) {
	Sheet sh;
	vector<string> ss(split(lines[0])); // "ss" means scroll & selection.
	sh.selx = Stoi(ss[0]);
	sh.sely = Stoi(ss[1]);
	sh.scx = Stoi(ss[2]);
	sh.scy = Stoi(ss[3]);
	lines[0] = "0 0 "; // Overwrite so that the file is not read incorrectly.
	for(string i : lines) {
		vector<string> cmds(split(i));
		string s;
		int start = cmds[0].length() + cmds[1].length() + 2;
		s.resize(start+1);
		for(int j = start-1; j < i.length(); j++) s[j-start] = i[j];
		sh.setData(Stoi(cmds[0]), Stoi(cmds[1]), s);
	}
	return sh;
}

string exportsheet(Sheet sh) {
	string res = "";
	res.append(Itos(sh.selx));
	res.append(" ");
	res.append(Itos(sh.sely));
	res.append(" ");
	res.append(Itos(sh.scx));
	res.append(" ");
	res.append(Itos(sh.scy));
	res.append("\n");
	
	for(pair<int, int> i : sh.cc) {
		cout << i.first << " " << i.second << endl;
		res.append(Itos(i.first));
		res.append(" ");
		res.append(Itos(i.second));
		res.append(" ");
		res.append(sh.getData(i.first, i.second));
		res.append("\n");
	}
	return res;
}

// Main.
int main() {
	Sheet sh;
	sh.selx = 0;
	sh.sely = 0;
	sh.scx = 0;
	sh.scy = 0;
	bool uzing = true;
	while(uzing) {
		// Show the spreadsheet.
		for(int i = 0; i < 80; i++) cout << "=";
		cout << "\n";
		for(int y = 0; y < 11; y++) {
			for(int x = 0; x < 8; x++) {
				string s = sh.getData(x+sh.scx, y+sh.scy);
				s.resize(9);
				for(int i = 0; i < 8; i++) 
					if(s[i] == '\0') s[i] = ' ';
				s[8] = '\0';
				cout << "=" << s;
			}
			cout << "=\n";
			for(int i = 0; i < 80; i++) cout << "=";
			cout << "\n";
		}
		// Get Input.
		string cmd;
		getline(cin, cmd);
		if(cmd.compare("exit") == 0) uzing = false;
		vector<string> cmds(split(cmd));
		if(cmds[0].compare("goto") == 0) {
			sh.selx = Stoi(cmds[1]);
			sh.sely = Stoi(cmds[2]);
			sh.scx = ((int)sh.selx/8)*8;
			sh.scy = ((int)sh.sely/11)*11;
		} else if(cmds[0].compare("edit") == 0) {
			string s;
			s.resize(cmd.length()-5);
			for(int i = 5; i < cmd.length()+1; i++) 
				s[i-5] = cmd[i];
			sh.setData(sh.selx, sh.sely, s);
		} else if(cmds[0].compare("open") == 0) {
			vector<string> lines(read(cmds[1]));
			sh = createsheet(lines);
		} else if(cmds[0].compare("write") == 0) {
			string s = exportsheet(sh);
			write(cmds[1], s);
		}
	}
}
