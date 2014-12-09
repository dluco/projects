/**
 * Based on:
 * 	"A hashtable in C", by Sourav Datta <soura_jagat@yahoo.com>
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LENGTH(x) (sizeof x / sizeof *x)

typedef struct _node {
	char *key;
	char *value;
	struct _node *next;
} node;

#define HASHSIZE 101
#define MULTIPLIER 31

static node *hashtable[HASHSIZE];

/*
 * Wrapper function for malloc.
 */
void *emalloc(size_t n)
{
	void *p;

	p = malloc(n);
	if (!p) {
		fprintf(stderr, "malloc of %zu bytes failed\n", n);
		exit(EXIT_FAILURE);
	}
	return p;
}

/*
 * Initialize all nodes in hastable to NULL.
 */
void init_hashtable()
{
	int i;
	for (i = 0; i < HASHSIZE; i++)
		hashtable[i] = NULL;
}

unsigned int hash(char *str)
{
	unsigned int h;
	unsigned char *p;

	h = 0;
	for (p = (unsigned char *) str; *p; p++)
		h = MULTIPLIER * h + *p;

	return h % HASHSIZE;
}

/*
 * Find the node containing the specified key.
 */
node *lookup(char *key)
{
	unsigned int hi;
	node *np;

	hi = hash(key);
	/* Traverse hash chain */
	for (np = hashtable[hi]; np != NULL; np = np->next) {
		if (!strcmp(np->key, key))
			break;
	}

	return np;
}

char *m_strdup(char *src)
{
	int length = strlen(src) + 1;
	char *dest = emalloc(length * sizeof *dest);
	strcpy(dest, src);

	return dest;
}

/*
 * Get the value for key.
 */
char *get(char *key)
{
	node *n = lookup(key);
	if (n)
		return n->value;
	else
		return NULL;
}

/*
 * Install a new key-value pair into the hashtable.
 */
node *install(char *key, char *value)
{
	unsigned int hi;
	node *np;

	if ((np = lookup(key)) == NULL) {
		/* Insert new key into table */
		hi = hash(key);
		np = emalloc(sizeof *np);
		np->key = m_strdup(key);
		/* Put new key at beginning of hash chain */
		np->next = hashtable[hi];
		hashtable[hi] = np;
	} else
		/* Key is already present - update value */
		free(np->value);

	np->value = m_strdup(value);

	return np;
}

/*
 * Displays the hashtable in (key,value) pairs.
 */
void display_table()
{
	int i;
	node *np;
	for (i = 0; i < HASHSIZE; i++) {
		if (hashtable[i]) {
			printf("{");
			for (np = hashtable[i]; np != NULL; np = np->next)
				printf("(%s,%s),", np->key, np->value);
			printf("}");
		}
	}
	printf("\n");
}

/*
 * Free all nodes and key-value pairs.
 */
void cleanup()
{
	int i;
	node *np, *next;

	for (i = 0; i < HASHSIZE; i++) {
		for (np = hashtable[i]; np != NULL; np = next) {
				next = np->next;
				free(np->key);
				free(np->value);
				free(np);
		}
	}
}

int main()
{
	int i;
	char *keys[] = { "name", "address", "phone", };
	char *values[] = { "David", "Canada", "7654321", };

	init_hashtable();

	printf("Installing key-value pairs into hashtable...\n");
	for (i = 0; i < LENGTH(keys); i++)
		install(keys[i], values[i]);
	printf("Done\n");

	printf("Test: %s=%s\n",
			values[LENGTH(values) - 1], get("k110"));

	install("phone", "1234567");

	printf("Test: %s and %s\n", get("name"), get("phone"));

	display_table();
	cleanup();
	return 0;
}
