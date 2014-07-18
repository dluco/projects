#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int main(int argc, char *argv[])
{
	int x = 0, y = 0;
	int max_x = 0, max_y = 0;
	int next_x = 0;
	int direction = 1;
	
	initscr(); // initialize the window
	noecho(); // don't echo keypresses
	curs_set(FALSE); // don't display a cursor

	for (;;) {
		getmaxyx(stdscr, max_y, max_x);
	
		clear();
		mvprintw(y, x, "o");
		refresh();

		usleep(DELAY);

		next_x = x + direction;
		
		if (next_x >= max_x || next_x < 0) {
			direction *= -1;
		} else {
			x += direction;
		}
	}

	endwin(); // restore normal terminal behaviour
}	
