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
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // IXON Disables Ctrl-S and Ctrl-Q allowing them to be read as bytes
  // Disables Ctrl-V
  // ICRNL Disables Ctrl-M being read and translated as a carriage return into a value of 13
  raw.c_oflag &= ~(OPOST);
  // Disables feautre to print into a carriage return and newline ("\r\n")
  raw.c_cflag |= (CS8);
  // Miscellaneous flags "BRKINT makes a break condition that sends a SIGINT signal to the program"
  // ,"INPCK enables parity checking", "ISTRIP strips the 8th bit of each input byte" 
  // , "CS8  is a bitmask that sets the character size to 8 bits per byte.
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  // ICANON disables canonical (inputs will be read byte by byte). This allows for the bitwise fliping 
  // of the bits in ECHO to prevent the echoing of inputs in the command line. ISIG allows for reading 
  // of Ctrl-C and Ctrl-Z.
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  // Sets the minimun number of bytes to run to 0
  // Sets the maximun amount of time to wait before read() returns;
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
  // Reads the current terminal settings (i.e, canonical mode, etc) and stores it in the raw termios
  // structure.  Reads the current 
  // terminal settings with the current settings and discards the unread
  // input. This is so only the relevant keystrokes are processed.


int main() { 
    enableRawMode();
  
    while (1) {
      char c = '\0';
      read(STDIN_FILENO, &c, 1); 
    // Reads 1 byte from standard input and stores it into c
    // as long as there are no more bytes to read.
    // Exits the input includes q
     if (iscntrl(c)) {
      printf("%d\r\n", c);
     } else {
      printf("%d ('%c')\r\n", c, c);
     }
     if (c =='q') break; 
     // If a control character (non printable) is selected representations
     // of bytes will be printed instead as both decimals (&d) and characters 
     // (&c). Starts a new line with carriage return.
     // Allows read to function with 0 bytes allowing for background processes
     // to continue even while waiting for an input.
    }
    return 0; 
}
