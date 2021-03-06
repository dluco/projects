/*
 * String Tokenizing:
 * 	the reentrant version of strtok, strtok_r, is used
 * 	to be thread safe.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct queue_node {
	char *data;
	struct queue_node *next;
};

struct queue {
	struct queue_node *head;
	struct queue_node *tail;
};

/*
 * initialize queue
 *
 * Return: none
 */
static void queue_init(struct queue *q)
{
	q->head = q->tail = NULL;
}

/*
 * test if queue is empty (zero elements)
 *
 * Return: non-zero if empty, 0 if non-empty
 */
static int queue_isempty(struct queue *q)
{
	return (q->head == NULL && q->tail == NULL);
}

/*
 * get the length of queue
 *
 * Return: length of queue
 */
static int queue_length(struct queue *q)
{
	struct queue_node *temp;
	int len;

	for (temp = q->head, len = 0; temp; temp = temp->next, len++);

	return len;
}

/*
 * add data to rear of queue
 *
 * Return: 0 on success, -1 on error
 */
static int queue_enqueue(struct queue *q, char *str)
{
	struct queue_node *temp;

	/* create new element */
	temp = malloc(sizeof *temp);
	if (!temp) {
		return -1;
	}
	temp->data = strdup(str);
	temp->next = NULL;

	if (queue_isempty(q)) {
		/* first element in queue */
		q->head = q->tail = temp;
	} else {
		/* add to rear */
		q->tail->next = temp;
		q->tail = temp;
	}

	return 0;
}

/*
 * remove data from front of queue
 *
 * Return: dynamically allocated string on success, NULL on error
 */
static char *queue_dequeue(struct queue *q)
{
	struct queue_node *temp;
	char *str;

	if (queue_isempty(q)) {
		return NULL;
	}

	/* advance head to second element */
	temp = q->head;	
	q->head = temp->next;

	/* get data */
	str = temp->data;

	free(temp);

	/* update tail if last element was removed */
	if (q->head == NULL) {
		q->tail = NULL;
	}

	return str;
}

/*
 * free a queue's data and delete all elements
 *
 * Return: none
 * */
static void queue_free(struct queue *q)
{
	struct queue_node *temp, *old;

	temp = q->head;

	while (temp) {
		free(temp->data);
		old = temp;
		temp = temp->next;
		free(old);
	}
}

/*
 * tokenize string on delimiters
 *
 * Return: null-terminated array of strings on success, NULL on error
 */
char **tokenize(char *str, char *delims)
{
	char *token, *s, *p, *saveptr;
	char **result;
	int i;
	struct queue q;

	/* make copy of input string */
	s = strdup(str);
	if (!s) {
		/* error */
		return NULL;
	}

	/* initialize queue */
	queue_init(&q);

	for (p = s; (token = strtok_r(p, delims, &saveptr)); p = NULL) {
		/* add to queue */
		queue_enqueue(&q, token);
	}

	free(s);

	/* length of queue is the number of tokens */
	result = malloc(queue_length(&q) * sizeof *result + 1);
	if (!result) {
		/* error */
		queue_free(&q);
		return NULL;	
	}

	for (i = 0; !queue_isempty(&q); i++) {
		/* empty out queue into result array */
		result[i] = queue_dequeue(&q);
	}
	result[i] = NULL;

	return result;
}

int main(void)
{
	char **split;
	int i;
	char *str = "one,two,three";
	split = tokenize(str, ",");

	for (i = 0; split[i]; i++) {
		printf("%s\n", split[i]);
		free(split[i]);
	}
	free(split);

	return 0;
}
