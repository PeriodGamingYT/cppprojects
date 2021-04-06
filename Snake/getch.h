#include <termios.h>
#include <unistd.h>
#include <stdio.h>

/* reads from keypress, doesn't echo */
int getch(int speed)
{
    struct termios oldattr, newattr;
    int ch = 0;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_iflag &= ~( IXON );
    newattr.c_lflag &= ~( ICANON | ECHO );
    newattr.c_cc[VMIN] = 0;
    newattr.c_cc[VTIME] = 1;
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    for(int i = 0; i < speed; i++) {
      int c = getchar();
      if(c >= 'a') ch = c;
    }
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
