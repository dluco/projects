#include <curses.h>

#define WIN_WIDTH 20
#define WIN_HEIGHT 3

int main(int argc, char *argv[])
{
	WINDOW *win;
	int maxx, maxy, offsetx, offsety;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(FALSE);

	refresh();
	getmaxyx(stdscr, maxy, maxx);

	offsetx = (maxx - WIN_WIDTH + 2) / 2;
	offsety = (maxy - WIN_HEIGHT - 2);
	
	win = newwin(WIN_HEIGHT + 2, WIN_WIDTH + 2, offsety, offsetx);
	box(win, 0, 0);
	scrollok(win, TRUE);

	mvwprintw(win, 1, 1, "Test string");
	wrefresh(win);

	getch();

	delwin(win);
	endwin();

	return 0;
}
