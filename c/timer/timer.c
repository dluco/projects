#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* sleep for the specified delay time  */
void sleep (time_t delay) {
	time_t time1, time2;

	time(&time1);

	do{
		time(&time2);
	} while ((time2 - time1) <= delay);
}

int main (int argc, char *argv[]) {
	int delay;
	
	if (strncmp(argv[1], "-h", 3) == 0) {
		fprintf(stdout, "Usage: timer [option]\n\n");
		fprintf(stdout, " -h\t display this help\n\n");
		return 1;
	}

	if (argc < 2) {
		printf("Delay: ");
		delay = 1;
	} else {
		delay = atoi(argv[1]);
	}

	sleep(delay);

	return 0;

}
