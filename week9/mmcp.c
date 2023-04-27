#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>

#define FILE_MODE (S_IRUSR | S_IWUSR)

void mperr(char *call, int val) {
    perror(call);
    exit(val);
}

int main(int argc, char *argv[]) {

    int fdin, fdout;
    caddr_t src, dst;
    struct stat statbuf;

    if (argc != 3) {
        fprintf(stderr, "usage: a.out <fromfile> <tofile>\n", 1);
    }

    if ( (fdin=open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "cannot open %s for writing", argv[1]);
        exit(2);
    }

    if ( (fdout=open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        fprintf(stderr, "cannot create %s for writing", argv[2]);
        exit(3);
    }

    if (fstat(fdin, &statbuf) < 0) {
        mperr("fstat error", 4);
    }

    if (lseek(fdout, statbuf.st_size-1, SEEK_SET) == -1) {
        mperr("lseek error", 5);
    }

    if (write(fdout, "", 1) != 1) {
        mperr("write error", 6);
    }

    return 0;
}