#include <string>

using namespace std;

class Cell {
public:
    string indt;
    string outdt;
    string format; // $dd.dd. are some examples
};

string format(string in, string f) {
    if(f == "\0") return in;
    string digits = "";
    string cts = " ";
    for(char i : in)
        if(i >= '0' && i <= '9') {
            cts[0] = i;
            digits.append(cts);
        }
    
    int dcount = 0;
    string ans = "";
    for(char i : f)
        if(i == 'd') {
            if(dcount < digits.length()) {
                cts[0] = digits[dcount];
                ans.append(cts);
                dcount++;
            } else ans.append("0");
        } else {
            cts[0] = i;
            ans.append(cts);
        }
    return ans;
}