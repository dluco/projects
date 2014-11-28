#include <ncurses.h>

#define DELAY 100

enum direction { UP, DOWN, RIGHT, LEFT, NODIR };

void update_position(int *y, int *x, int *dir);

int main(int argc, char *argv[])
{
	int x, y;
	int ch, dir;

	initscr();
	cbreak();
	noecho();
	timeout(DELAY);
	keypad(stdscr, TRUE);
	curs_set(FALSE);

	y = LINES/2;
	x = COLS/2;
	mvaddch(y, x, 'X');
	refresh();

	getch();
	while ((ch = getch()) != 'q') {
		switch (ch) {
		case KEY_RIGHT:
			dir = RIGHT;
			break;
		case KEY_LEFT:
			dir = LEFT;
			break;
		case ' ':
			dir = UP;
			break;
		default:
		//	dir = NODIR;
			break;
		}
		clear();
		update_position(&y, &x, &dir);
		mvaddch(y, x, 'X');
		refresh();
	}

	endwin();

	return 0;
}

void update_position(int *y, int *x, int *dir)
{
	int next_y = *y;
	int next_x = *x;
	int max_y, max_x;

	/* TODO: reset position if below ground */

	/* allow movement if not in air */
	if (*y = LINES/2) {
		switch (*dir) {
		case UP:
			next_y -= 1;
			break;
		case RIGHT:
			next_x += 1;
			break;
		case LEFT:
			next_x -= 1;
			break;
		default:
			break;
		}
	} else {
		/* in air - rising or falling? */
		switch (*dir) {
		case UP:
			/* check if at max height */
			if (*y <= (LINES/2 - 5)) {
				*dir = DOWN;
				next_y += 1;
			} else {
				/* continue upwards */
				next_y -= 1;
			}
			break;
		case DOWN:
			next_y += 1;
		}
	}

	getmaxyx(stdscr, max_y, max_x);

	if (!(next_y >= max_y || next_y < 0 || next_x >= max_x || next_x < 0)) {
		/* cursor is within window */
		*y = next_y;
		*x = next_x;
	}
}
