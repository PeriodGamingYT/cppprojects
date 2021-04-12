#include <termios.h>
#include <unistd.h>
#include <stdio.h>

struct termios oldattr;

void init() {
    tcgetattr( STDIN_FILENO, &oldattr );
    struct termios newattr;
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    newattr.c_cc[VMIN] = 0;
    newattr.c_cc[VTIME] = 1;
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &newattr );
}

/* reads from keypress, doesn't echo */
int getch()
{
    int ch = 0;
    read(STDIN_FILENO, &ch, 1);
    return ch;
}

void reset() {
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &oldattr );
}