#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>

#define DELAY 150

#define GROUND (3*(LINES/4))
#define MAX_HEIGHT (GROUND-(LINES/2))

typedef enum _direction direction;
typedef struct _Object Object;

enum _direction { UP, DOWN, RIGHT, LEFT, NODIR };

struct _Object {
	int item;
	int y;
	int x;
	float v_y;
	float v_x;
	direction dir;
};

void update_position(WINDOW *win, Object *object, direction next_dir);

void draw_object(WINDOW *win, Object *object);

int main(int argc, char *argv[])
{
	Object *cursor;
	direction next_dir = NODIR;
	int ch;

	initscr();
	cbreak();
	noecho();
	timeout(DELAY);
	keypad(stdscr, TRUE);
	curs_set(FALSE);

	cursor = malloc(sizeof(*cursor));
	if (!cursor) {
		fprintf(stderr, "failed to allocate object\n");
		exit(EXIT_FAILURE);
	}

	cursor->item = 'X';
	cursor->y = GROUND;
	cursor->x = COLS/2;
	cursor->v_y = 0.0;
	cursor->v_x = 0.0;
	cursor->dir = NODIR;

	mvwaddch(stdscr, cursor->y, cursor->x, cursor->item);
	refresh();

	while ((ch = getch()) != 'q') {
		switch (ch) {
		case KEY_RIGHT:
			next_dir = RIGHT;
			break;
		case KEY_LEFT:
			next_dir = LEFT;
			break;
		case ' ':
			next_dir = UP;
			break;
		default:
			next_dir = NODIR;
			break;
		}
		clear();
		update_position(NULL, cursor, next_dir);
		draw_object(NULL, cursor);
		refresh();
	}

	endwin();

	free(cursor);

	return 0;
}

void update_position(WINDOW *win, Object *object, direction next_dir)
{
	int next_y, next_x;
	int max_y, max_x;

	if (win == NULL) {
		win = stdscr;
	}

	/* reset position if below ground */
	if (object->y > GROUND) {
		object->y = GROUND;
	}

	getmaxyx(win, max_y, max_x);

	next_y = object->y;
	next_x = object->x;

	switch (next_dir) {
	case UP:
		/* allow jump if not in air */
		if (object->y == GROUND) {
			next_y -= 1;
			object->dir = UP;
		}
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
	if (object->y < GROUND) {
		/* in air - rising or falling? */
		switch (object->dir) {
		case UP:
			/* check if at max height */
			if (object->y == MAX_HEIGHT) {
				object->dir = DOWN;
				next_y += 1;
			} else {
				/* continue upwards */
				next_y -= 1;
			}
			break;
		case DOWN:
			next_y += 1;
			break;
		}
	}

	if (!(next_y >= max_y || next_y < 0 || next_x >= max_x || next_x < 0)) {
		/* cursor is within window */
		object->y = next_y;
		object->x = next_x;
	}
}

void draw_object(WINDOW *win, Object *object)
{
	if (win == NULL) {
		win = stdscr;
	}

	mvwaddch(win, object->y, object->x, object->item);
}
