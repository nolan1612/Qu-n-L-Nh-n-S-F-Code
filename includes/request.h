#ifndef REQUEST_H
#define REQUEST_H

#define MAX_REQUESTS 500

typedef struct {
    char mssv[20];
    char name[50];
} Request;

int loadRequests(Request list[]);
void saveRequests(Request list[], int count);
void sendUnlockRequest(Account *currentAcc, int accountCount);
void viewUnlockRequests(Account list[], int accountCount);
void deleteUnlockRequest(Request requests[], int requestCount, char mssv[]);

#endif
