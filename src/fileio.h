#ifndef FILEIO_H
#define FILEIO_H
#endif

typedef struct{
    char username[20];
    char password[20];
    int role;
}Account;

int saveAccount(Account arr[], int count);

int loadAccount(Account arr[]);

