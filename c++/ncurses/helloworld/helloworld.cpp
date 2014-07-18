#include <curses.h>
#include <string.h>

#define BUFFER 64

int main()
{
	char mesg[] = "Enter your age: ";
	char input[BUFFER];
	int row, col;
	
	initscr();
	do {
		clear();
		getmaxyx(stdscr, row, col);
		mvprintw(row/2, (col - strlen(mesg))/2, "%s", mesg);
		
		getstr(input);
		mvprintw(LINES -1, 0, "You entered: %s", input);
	} while (getch() != 'q');
	endwin();
	
	return 0;
}
