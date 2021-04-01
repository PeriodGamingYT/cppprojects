#include <vector>
#include <string>

using namespace std;

class v2 {
	public:
		int x;
		int y;
};

v2 gv2(int x, int y) {
	v2 res;
	res.x = x;
	res.y = y;
	return res;
}

class Path {
	public:
		vector<v2> data;
		string name;
		char symbol;
        int r, g, b;
        char pchar;
	void addData(v2 a) {
		data.resize(data.size()+1);
		data[data.size()-1] = a;
	}

	void removeData(int idx) {
		data.erase(data.begin()+idx);
	}
};

