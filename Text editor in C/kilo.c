#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
 
struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
// Disbles raw mode (inputs are echoed again allowing for a clean process)

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  struct termios raw = orig_termios;  
  raw.c_lflag &= ~(ECHO);
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
// Reads the current terminal settings (i.e, canonical mode, etc) and stores it in the raw termios
// structure. This allows for the bitwise fliping of the bits in ECHO to 
// disable the echoing of inputs in the command line. Reads the current 
// terminal settings with the current settings and discards the unread
// input. This is so only the relevant keystrokes are processed.


int main() 
{ 
    enableRawMode();
    
    char c; 
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'); 
    return 0; 
}
// Reads 1 byte from standard input and stores it into c
// as long as there are no more bytes to read.
// Exits the input includes q