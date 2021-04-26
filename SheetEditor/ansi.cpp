#include <iostream>

using namespace std;

void setpos(int y, int x) { // Flipped because ANSI is weird.
    cout << "\x1b[" << x << ";" << y << "f";
}

void contrast() {
    cout << "\x1b[7m";
}

void normal() {
    cout << "\x1b[0m";
}