#include <utility>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;

int getnlength(int n) {
    int c = 1;
    int ans = 0;
    while(n > c) {
        c *= 10;
        ans++;
    }
    return ans;
}

int getn(int start, int base, string s) {
    int c = 1;
    int ans = 0;
    for(int i = s.length()-1; i >= 0; i--) {
        ans += (s[i]-start)*c;
        c *= base;
    }
    return ans;
}

float getdeci(string s) {
    bool neg = s[0] == '-';

    int doti = neg ? 1 : 0;
    for(char i : s) {
        doti++;
        if(i == '.') break;
    }
    if(doti >= s.length()-1)
        return (float)getn('0', 10, s);

    int whole = getn('0', 10, s.substr(0, doti-1));
    int deci = getn('0', 10, s.substr(doti, s.length()-1));
    float ans = whole + (deci/pow(10, getnlength(deci)));
    return neg ? ans*-1 : ans;
}

pair<int, int> getlncords(string s) {
    int spliti = 0;
    for(int i = 0; i < s.length(); i++)
        if(s[i] <= '9') spliti = i;
    return make_pair(getn('a', 26, s.substr(0, spliti)),
                     getn('0', 10, s.substr(spliti, s.length()-1))-1);
}