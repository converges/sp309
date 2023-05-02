#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define TRUE    1
#define FALSE   0

#define NUM_RECORDS 100

struct record {
    char name[20];
    int id;
    int balance;
};

int reclock (int fd, int recno, int len, int type);
void display_record(struct record *curr);

int main(void) {
    struct record current, to_account;
    int record_no, to_record_no;
    int fd, pos, i, n;
    char yes;
    char operation;
    int amount;
    char buffer[100];
    int quit=FALSE;

    fd = open("./account", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    while (1) {
        printf("Enter account number (0-99): ");
        scanf("%d", &record_no);
        fgets(buffer, 100, stdin);

        if (record_no<0 && record_no >= NUM_RECORDS) {
            break;
        }
        printf("Enter operation name (c/r/d/w/t/q): ");
        scanf("%c", &operation);

        switch (operation) {
            case 'c': // create
                reclock(fd, record_no, sizeof(struct record), F_WRLCK);
                pos = record_no * sizeof(struct record);
                lseek(fd, pos, SEEK_SET);
                printf("> id ? ");
                scanf("%d", &current.id);
                printf("> name ? ");
                scanf("%s", current.name);
                current.balance = 0;
                n = write(fd, &current, sizeof(struct record));
                display_record(&current);
                reclock(fd, record_no, sizeof(struct record), F_UNLCK);
                break;
            case 'r': // inquiry
                reclock(fd, record_no, sizeof(struct record), F_RDLCK);
                pos = record_no * sizeof(struct record);
                lseek(fd, pos, SEEK_SET);
                n = read(fd, &current, sizeof(struct record));
                display_record(&current);
                reclock(fd, record_no, sizeof(struct record), F_UNLCK);
                break;
            case 'd': // deposit
                reclock(fd, record_no, sizeof(struct record), F_WRLCK);
                pos = record_no * sizeof(struct record);
                lseek(fd, pos, SEEK_SET);
                n = read(fd, &current, sizeof(struct record));
                display_record(&current);
                printf("Enter amount: ");
                scanf("%d", &amount);
                current.balance += amount;
                lseek(fd, pos, SEEK_SET);
                write(fd, &current, sizeof(struct record));
                reclock(fd, record_no, sizeof(struct record), F_UNLCK);
                break;
            case 'w': // withdraw
                reclock(fd, record_no, sizeof(struct record), F_WRLCK);
                pos = record_no * sizeof(struct record);
                lseek(fd, pos, SEEK_SET);
                n = read(fd, &current, sizeof(struct record));
                display_record(&current);
                printf("Enter amount: ");
                scanf("%d", &amount);
                if (current.balance < amount) {
                    printf("Not enough balance\n");
                }
                else {
                    current.balance -= amount;
                    lseek(fd, pos, SEEK_SET);
                    write(fd, &current, sizeof(struct record));
                }
                reclock(fd, record_no, sizeof(struct record), F_UNLCK);
                break;
            case 't': // transfer

                printf("Enter destination account number (0-99): ");
                scanf("%d", &to_record_no);
                fgets(buffer, 100, stdin);
                if (to_record_no < 0 || to_record_no >= NUM_RECORDS) {
                    printf("Invalid account number\n");
                    break;
                }
                if (to_record_no == record_no) {
                    printf("Cannot transfer to the same account\n");
                    break;
                }
                reclock(fd, record_no, sizeof(struct record), F_WRLCK);
                pos = record_no * sizeof(struct record);
                lseek(fd, pos, SEEK_SET);
                n = read(fd, &current, sizeof(struct record));
                display_record(&current);
                printf("Enter amount: ");
                scanf("%d", &amount);
                if (current.balance < amount) {
                    printf("Not enough balance\n");
                }
                else {
                    reclock(fd, to_record_no, sizeof(struct record), F_WRLCK);
                    pos = to_record_no * sizeof(struct record);
                    lseek(fd, pos, SEEK_SET);
                    n = read(fd, &to_account, sizeof(struct record));
                    to_account.balance += amount;
                    lseek(fd, pos, SEEK_SET);
                    write(fd, &to_account, sizeof(struct record));
                    current.balance -= amount;
                    lseek(fd, record_no * sizeof(struct record), SEEK_SET);
                    write(fd, &current, sizeof(struct record));
                    display_record(&current);
                    reclock(fd, to_record_no, sizeof(struct record), F_UNLCK);
                }

                reclock(fd, record_no, sizeof(struct record), F_UNLCK);
                break;
            case 'q':
                quit = TRUE;
                break;
            default:
                printf("illegal input\n");
                continue;
        }
    }
    close(fd);
    fflush(NULL);

    return 0;
}

int reclock(int fd, int recno, int len, int type) {

    struct flock fl;
    switch (type) {
        case F_RDLCK:
        case F_WRLCK:
        case F_UNLCK:
            fl.l_type = type;
            fl.l_whence = SEEK_SET;
            fl.l_start = recno * len;
            fl.l_len = len;
            fcntl(fd, F_SETLKW, &fl);
            return 1;
            break;
        default:
            return -1;
    }
}

void display_record(struct record *curr) {
    printf("\n");
    printf("id: %d \n", curr->id);
    printf("name: %s \n", curr->name);
    printf("balance: %d \n", curr->balance);
    printf("\n");
}