#include "lnlib.cpp"
#include <vector>
#include <string>

vector<string> split(string s) {
    vector<string> ans(0);
    string token = "";
    string cts = " ";
    string ops = "+-(),*/";
    for(char i : s) {
        bool split = false;
        for(char j : ops)
            if(i == j) split = true;
        cts[0] = i;
        if(split) {
            ans.insert(ans.end(), token);
            ans.insert(ans.end(), cts);
            token = "";
        } else
            token.append(cts);
    }
    ans.insert(ans.end(), token);
    return ans;
}

float solve(vector<string> s) {
    float ans = getdeci(s[0]);
    for(int i = 0; i < s.size(); i++) {
        if(s[i][0] == '+') ans += getdeci(s[i+1]);
        if(s[i][0] == '-') ans -= getdeci(s[i+1]);
        if(s[i][0] == '*') ans *= getdeci(s[i+1]);
        if(s[i][0] == '/') ans /= getdeci(s[i+1]);
    }
    return ans;
}