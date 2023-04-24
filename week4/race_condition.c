#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {

    int fd1, fd2, n;
    char buf;

    fd1 = open("myfile", O_RDONLY);
    fd2 = creat("copyfile", 0777);
    
    fork();
    // parent & child shares open files and their file pointers (r/w offset)
    // if a parent get a char form the file then the child get the next char
    // because they share the same file pointer

    while ( (n=read(fd1, &buf, 1)) != 0) {
        write(fd2, &buf, n);
    }

    close(fd1);
    close(fd2);

    return 0;
}