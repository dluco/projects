#include <curses.h>
#include <signal.h>
#include <string.h>

#define BUFFER 64

void resize_handler(int);

int main()
{
	char input[BUFFER];
	WINDOW *def;
	
	initscr();
	signal(SIGWINCH, resize_handler);

	def = newwin(5, 5, 5, 5);

//	noecho();
	scrollok(def, TRUE);
//	wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);

	do {
		wprintw(def, "Input: ");	
		wgetstr(def, input);
		wprintw(def, "You entered: %s\n", input);
	} while (strncmp(input, "q\0", 2) != 0);

	endwin();
	delwin(def);
	
	return 0;
}

void resize_handler(int sig)
{
	printw("window resized\n");
	refresh();
}
