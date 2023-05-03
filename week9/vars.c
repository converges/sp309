#include <stdio.h>
#include <stdlib.h>

/* Data Area */
int a=1;
static int b=2;

void fun(int n);

int main(void) {

	int c=3; // Stack
	static int d = 4; // Data
	char *p; //Stack
	
	p=(char *)malloc(40); // &p is in Heap
	fun(5);

	return 0;
}

/* m, n are allocated in the Stack */
void fun(int n) {
	int m=6;
}
