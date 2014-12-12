#include <stdlib.h>
#include <X11/Xlib.h>
#include <unistd.h>

int main()
{
	Display *dpy;
	Window w;

	if (!(dpy = XOpenDisplay(NULL))) {
		exit(EXIT_FAILURE);
	}

	w = XCreateWindow(dpy, DefaultRootWindow(dpy), 0, 0,
			200, 100, 0,
			CopyFromParent, CopyFromParent, CopyFromParent,
			0, 0);

	XMapWindow(dpy, w);
	XFlush(dpy);

	sleep(10);

	return 0;
}
