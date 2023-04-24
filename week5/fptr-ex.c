#include <stdio.h>

void greet();
void caller(void (*myFunction) (void));

int main(void) {

    void (*sayHello) (void);

    printf("Calling greet() directly.\n");
    greet();

    printf("\n");

    printf("Calling greet() via a function pointer.\n");
    sayHello = greet;
    sayHello();
    printf("\n");

    printf("Calling greet() via another function.\n");
    caller(greet);

    return 0;
}

void greet() {
    printf("Hello, world!\n");
}

void caller(void (*myFunction) (void)) {
    myFunction();
}