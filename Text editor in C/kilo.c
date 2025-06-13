#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
 
struct termios orig_termios;
// Stores a backup of the orignal terminal settings

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
// Disbles raw mode (inputs are echoed again allowing for a clean process)

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  // Reads the current terminal settings (i.e, canonical mode, etc) and stores it in the raw/orig_termios structure.
  atexit(disableRawMode);

  struct termios raw = orig_termios; 
  // Saves raw as the original terminal settings
  raw.c_lflag &= ~(ECHO | ICANON);
// Turns off canonical (inputs will be read byte by byte). This allows for the bitwise fliping of the bits in ECHO to 
// disable the echoing of inputs in the command line.
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
// Reads the current terminal settings (i.e, canonical mode, etc) and stores it in the raw termios
// structure.  Reads the current 
// terminal settings with the current settings and discards the unread
// input. This is so only the relevant keystrokes are processed.


int main() { 
    enableRawMode();
    
    char c; 
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    // Reads 1 byte from standard input and stores it into c
    // as long as there are no more bytes to read.
    // Exits the input includes q
     if (iscntrl(c)) {
      printf("%d\n", c);
     } else {
      printf("%d ('%c')\n", c, c);
     }
     // If a control character (non printable) is selected representations
     // of bytes will be printed instead as both decimals (&d) and characters 
     // (&c)
    }
    return 0; 
}
