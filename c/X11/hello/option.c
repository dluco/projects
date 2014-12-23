#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define LENGTH(x) (sizeof x / sizeof *x)

struct _opt_entry {
	char *long_name;
	char short_name;
	char *description;
};

typedef struct _opt_entry OptEntry;

void arg_parse(int argc, char **argv, OptEntry *entries)
{
	int i, j;
	bool continue_parsing = true;

	for (i = 0; i < argc; i++) {
		char *arg;
		if (argv[i][0] == '-' && argv[i][1] != '\0' && continue_parsing) {
			if (argv[i][1] == '-') {
				/* long option */
				arg = argv[i]+ 2;

				if (*arg == 0) {
					/* '--' stops further parsing */
					continue_parsing = false;
					continue;
				}

				for (j = 0; entries[j].long_name != NULL; j++) {
					if (strcmp(arg, entries[j].long_name) == 0) {
						printf("long option found: %s\n", arg);
					}
				}
			} else {
				/* short option */
				arg = argv[i] + 1;

				for (j = 0; entries[j].short_name != 0; j++) {
					if (*arg == entries[j].short_name) {
						printf("short option found: %s\n", arg);
					}
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	OptEntry entries[] = {
		{ "help", 'h', "Print help info" },
		{ "version", 'v', "Print version info" },
		{ NULL }
	};

	arg_parse(argc, argv, entries);

	return 0;
}
