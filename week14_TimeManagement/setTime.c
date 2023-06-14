#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

    time_t curtime, modtime;

    if (argc != 3) {
        perror("Argument Error");
        printf("format: [+|-]sec");
        exit(1);
    }

    modtime = (time_t) time(&curtime);

    printf("current time: %d sec from 1920, 1, 1 00 : 00: 00\n", (int)curtime);

    if (!strcmp(argv[1], "-")) {
        modtime = curtime - atoi(argv[2]);
    }
    else if (!strcmp(argv[1], "+")) {
        modtime = curtime + atoi(argv[2]);
    }
    else {
        perror("Argument Error");
        exit(2);
    }
    printf("\n");
    
    /* stime() has been deprecated
    if (stime(&modtime) == -1) {
        perror("stime Error");
    }
    printf("modified time: %d sec from 1970, 1, 1 00 : 00 : 00\n", (int) modtime);
    */

    return 0;
}
