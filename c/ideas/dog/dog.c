#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char ch;
	FILE **files;
	int i;

	if (argc < 2) {
		printf("dog: no input file specified\n");
		return 1;
	}

	files = malloc(sizeof(FILE) * (argc - 1));
	if (!files) {
		fprintf(stderr, "malloc error\n");
		return 1;
	}

	for (i = 0; i < (argc - 1); i++) {
		printf("%s\n", argv[i+1]);
		files[i] = fopen(argv[i+1], "r");
		if (!files[i]) {
			fprintf(stderr, "fopen error\n");
			return 1;
		}
		printf("dog: file opened\n");
	}

	for (i = 0; i < (argc - 1); i++) {
		while ((ch = fgetc(files[i])) != EOF) {
			printf("%c", ch);
		}
	}

	for (i = 0; i < (argc - 1); i++) {
		if (fclose(files[i])) {
			fprintf(stderr, "fclose error\n");
		}
		printf("dog: file closed\n");
	}
	
	return 0;
}
