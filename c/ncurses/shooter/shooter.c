#include <curses.h>
#include <unistd.h>

#define DELAY 50

#define SHIP '^'
#define MISSILE '!'
#define ALIEN 'W'
#define NUM_ALIENS 25

enum direction { UP, DOWN, LEFT, RIGHT, STAY };

bool collision(int, int, int, int);
bool update_missile(int, int);
void update_craft(int*, int*, int*, int);

int main(int argc, char *argv[])
{
	int ship_x, ship_y;
	int mis_x = 1, mis_y = 1;
	int alien_x[NUM_ALIENS], alien_y[NUM_ALIENS];
	bool alien[NUM_ALIENS];
	int n_aliens = NUM_ALIENS;
	int alien_dir = RIGHT, ship_dir = STAY;
	int i, ch;
	bool missile = FALSE;

	initscr();
	cbreak();
	noecho();
	timeout(DELAY);
	keypad(stdscr, TRUE);
	curs_set(FALSE);

	ship_y = LINES - 3;
	ship_x = COLS / 2;

	for (i = 0; i < NUM_ALIENS; i++) {
		alien_y[i] = 0;
		alien_x[i] = i;
		alien[i] = TRUE;
	}

	mvaddch(ship_y, ship_x, SHIP);
	refresh();

	while((ch = getch()) != 'q') {
		clear();
		for (i = 0; i < NUM_ALIENS; i++) {
			if (alien[i] && collision(alien_y[i], alien_x[i], mis_y, mis_x)) {
				alien[i] = FALSE;
				n_aliens--;
			}
		}
		if (n_aliens == 0) {
			mvprintw(LINES / 2, (COLS - 10) / 2, "YOU WIN!!!");
			refresh();
			sleep(5);
			goto end;
		}
		switch(ch) {
		case ' ':
			if (!missile) {
				mis_x = ship_x;
				mis_y = ship_y;
				missile = TRUE;
			}
		default:
			ship_dir = STAY;
			break;
		case KEY_RIGHT:
			ship_dir = RIGHT;
			break;
		case KEY_LEFT:
			ship_dir = LEFT;
			break;
		}
		
		if (missile) {
			missile = update_missile(--mis_y, mis_x);
		} else {
			mis_x = ship_x;
			mis_y = ship_y;
		}
//		missile = (missile) ? update_missile(--mis_y, mis_x) : FALSE;
		update_craft(&ship_y, &ship_x, &ship_dir, SHIP);
		for (i = 0; i < NUM_ALIENS; i++) {
			if (alien[i]) {
				update_craft(&alien_y[i], &alien_x[i], &alien_dir, ALIEN);
			}
		}

		refresh();
	}

end:
	endwin();
	return 0;
}

bool collision(int alien_y, int alien_x, int mis_y, int mis_x)
{
	return (alien_y == mis_y && alien_x == mis_x) ? TRUE : FALSE;
}

bool update_missile(int y, int x)
{
	int max_y, max_x;

	getmaxyx(stdscr, max_y, max_x);

	if (!(x >= max_x || x <= 0 || y >= max_y || y <= 0)) {
		mvaddch(y, x, MISSILE);
		return TRUE;
	}
	return FALSE;
}

void update_craft(int *y, int *x, int *direction, int item)
{
	int x_t = *x;
	int max_x;

	switch(*direction) {
	case LEFT:
		x_t -= 1;
		break;
	case RIGHT:
		x_t += 1;
		break;
	default:	/* don't move */
		break;
	}

	max_x = getmaxx(stdscr);

	if (!(x_t >= max_x || x_t < 0)) {
		/* cursor within window */
		mvaddch(*y, x_t, item);
		*x = x_t;
		return;
	} else if (item == ALIEN) {
		/* alien - change direction */
		*direction  = (*direction == RIGHT) ? LEFT : RIGHT;
	}
	mvaddch(*y, *x, item);
}
