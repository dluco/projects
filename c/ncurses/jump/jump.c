/*
 * Compile with:
 * 	gcc -o jump jump.c -lncurses
 */


/*
 * TODO: use a char** to hold item?
 */

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELAY 150

#define GROUND (3*(LINES/4))
#define MAX_HEIGHT (GROUND-(LINES/2))

typedef enum _direction direction;
typedef enum _color color;
typedef struct _Object Object;

enum _direction { UP, DOWN, RIGHT, LEFT, NODIR };
enum _color { CURSOR_COLOR=1 };

struct _Object {
	char *item;
	int height;
	int width;
	int y;
	int x;
	float v_y;
	float v_x;
	direction dir;
	color colr;
};

void update_position(WINDOW *win, Object *object, direction next_dir);

int object_height(Object *object);
int object_width(Object *object);

void draw_object(WINDOW *win, Object *object);

int main(int argc, char *argv[])
{
	Object *cursor;
	direction next_dir = NODIR;
	int ch;

	initscr(); /* Start curses mode */

	cbreak();
	noecho();
	timeout(DELAY);
	keypad(stdscr, TRUE);
	curs_set(FALSE);

	start_color(); /* Start color mode */

	init_pair(CURSOR_COLOR, COLOR_RED, COLOR_BLACK);

	cursor = malloc(sizeof(*cursor));
	if (!cursor) {
		fprintf(stderr, "failed to allocate object\n");
		exit(EXIT_FAILURE);
	}

	cursor->item = "XXX";
	cursor->height = object_height(cursor);
	cursor->width = object_width(cursor);;
	cursor->y = GROUND;
	cursor->x = COLS/2;
	cursor->v_y = 1.0;
	cursor->v_x = 1.0;
	cursor->dir = NODIR;
	cursor->colr = CURSOR_COLOR;

	draw_object(NULL, cursor);
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
			next_y -= (int)object->v_y;
			object->dir = UP;
		}
		break;
	case RIGHT:
		next_x += (int)object->v_x;
		break;
	case LEFT:
		next_x -= (int)object->v_x;
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
				next_y += (int)object->v_y;
			} else {
				/* continue upwards */
				next_y -= (int)object->v_y;
			}
			break;
		case DOWN:
			next_y += 1;
			break;
		}
	}

	if (!((next_y + object->height - 1) >= max_y || next_y < 0)) {
		/* cursor is within window */
		object->y = next_y;
	}

	if (!((next_x + object->width - 1) >= max_x || next_x < 0)) {
		/* cursor is within window */
		object->x = next_x;
	}
}

int object_height(Object *object)
{
	char *t;
	int i;

	for (t = object->item, i = 1; *t; t++) {
		if (*t == '\n') {
			i++;
		}
	}

	return i;
	/*
	return 1;
	*/
}

int object_width(Object *object)
{
	return strlen(object->item);
}

void draw_object(WINDOW *win, Object *object)
{
	if (win == NULL) {
		win = stdscr;
	}

	attron(COLOR_PAIR(object->colr));
	mvwprintw(win, object->y, object->x, "%s", object->item);
	attroff(COLOR_PAIR(object->colr));
}
