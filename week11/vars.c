#include <stdio.h>
#include <stdlib.h>

void fun(int n);

int a = 1;
static int b = 2;

int main() {
    int c = 3;
    static int d = 4;
    char *p;

    p = (char *)malloc(40);
    fun(5);

    return 0;
}

void fun(int n) {
    int m = 6;
}