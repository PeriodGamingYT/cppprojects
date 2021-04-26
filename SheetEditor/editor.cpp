#include "sheet.cpp"
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <sstream>
#include "expression.cpp"

vector<string> formatToExp(string S, Sheet sh) {
    vector<string> s(split(S));
    // I am too lazy to rewrite j to i and vice versa.
    for(int j = 0; j < s.size(); j++) {
        string i = s[j];
        if(i[0] >= 'a' && i[0] <= 'z') 
            s[j] = sh.cells[getlncords(i)].indt;
    }
    return s;
}