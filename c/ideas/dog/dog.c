#include <stdio.h>
#include <stdlib.h>

#define PROGRAM "dog"

int main(int argc, char *argv[])
{
	char ch;
	FILE *fp;
	int i;

	if (argc < 2) {
		fp = stdin;
		while ((ch = fgetc(fp)) != EOF) {
			printf("%c", ch);
		}
		return EXIT_SUCCESS;
	}

	for (i = 0; i < (argc - 1); i++) {
		if (!(fp = fopen(argv[i+1], "r"))) {
			fprintf(stderr, "%s: fopen error\n", PROGRAM);
			return EXIT_FAILURE;
		} else {
			printf("file opened\n");
		}

		while ((ch = fgetc(fp)) != EOF) {
			printf("%c", ch);
		}

		if (fclose(fp)) {
			fprintf(stderr, "%s: fclose error\n", PROGRAM);
			return EXIT_FAILURE;
		} else {
			printf("file closed\n");
		}
	}

	return EXIT_SUCCESS;
}
