#ifndef __OPTION_H__
#define __OPTION_H__

typedef struct _OptEntry OptEntry;

struct _OptEntry {
	char *long_name;
	char short_name;
	char *description;
};

#endif /* __OPTION_H__ */
