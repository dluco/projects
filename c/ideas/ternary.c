/* preprocessor directives */
#include <stdio.h>
#define MAX 10

/* function prototype */
void foo(void);

int main()
{
//	(1 == 2) ? printf("TRUE\n") : printf("FALSE\n");
	foo();

	return 0;
}

void foo(void)
{
	if (1 == MAX) {
		printf("TRUE\n");
	} else {
		printf("FALSE\n");
	}
}	
