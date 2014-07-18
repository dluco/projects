#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

enum direction { UP, DOWN, RIGHT, LEFT };

void move_cursor(int*, int*, int);

int main(int argc, char *argv[])
{
	int x = 0, y = 0;
	int ch, dir;
	
	initscr(); // initialize the window
	noecho(); // don't echo keypresses
	cbreak(); // don't wait for newline
	curs_set(FALSE); // don't display a cursor
	keypad(stdscr, TRUE); // allow capture of special keys

	mvaddch(0, 0, 'X');
	refresh();

	while ((ch = getch()) != 'q') {
		switch (ch) {
		case KEY_UP:
			dir = UP;
			break;
		case KEY_DOWN:
			dir = DOWN;
			break;
		case KEY_RIGHT:
			dir = RIGHT;
			break;
		case KEY_LEFT:
			dir = LEFT;
			break;
		default:
			break;
		}
		move_cursor(&x, &y, dir);
	}

	endwin(); // restore normal terminal behaviour

	return 0;
}

void move_cursor(int *x, int *y, int direction)
{
	int next_x = *x;
	int next_y = *y;
	int max_y, max_x;

	switch(direction) {
	case UP:
		next_y -= 1;
		break;
	case DOWN:
		next_y += 1;
		break;
	case RIGHT:
		next_x += 1;
		break;
	case LEFT:
		next_x -= 1;
	default:
		break;
	}

	getmaxyx(stdscr, max_y, max_x);
	
	if (!(next_x >= max_x || next_x < 0 || next_y >= max_y || next_y < 0)) {
		/* cursor is still within window */
		clear();
		mvaddch(next_y, next_x, 'X');
		refresh();
		*x = next_x;
		*y = next_y;
	}
}
