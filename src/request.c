#include <stdio.h>
#include <string.h>
#include "../includes/request.h"
#include "../includes/auth.h"
#include "../includes/utils.h"
#include "../includes/fileio.h"


int loadRequests(Request list[]) {
    FILE *f = fopen("data/request.dat", "r");
    if (f == NULL) return 0;
    int count = 0;
    while (fread(&list[count], sizeof(Request), 1, f)) count++;
    fclose(f);
    return count;
}


void saveRequest(Request list[], int count) {
    FILE *f = fopen("data/request.dat", "w");
    if (f == NULL) {
        printf("\033[1;31m[Error]\033[0m Can not open file request.dat for writing!\n");
        return;
    }
    size_t written = fwrite(list, sizeof(Request), count, f);
    
    if (written != (size_t)count) {
        printf("\033[1;31m[Error]\033[0m Failed to write all requests to file!\n");
    }
    
    fclose(f);
}



void sendUnlockRequest(Account *currentAcc, int accountCount) {
    if (currentAcc->isLocked == 0) {
        printf(">> Your account is not locked!\n");
        return;
    }

    // Kiểm tra đã gửi yêu cầu chưa
    Request list[MAX_REQUESTS];
    int requestCount = loadRequests(list);

    for (int i = 0; i < requestCount; i++) {
        if (strcmp(list[i].mssv, currentAcc->studentid) == 0) {
            printf(">> You have already submitted a request, please wait for the admin to review!\n");
            return;
        }
    }

    if (!confirmAction("Confirm sending the unlock request?")) {
        printf(">> Request cancelled.\n");
        return;
    }

    Request req;
    strcpy(req.name, currentAcc->username);
    strcpy(req.mssv, currentAcc->studentid);

    list[requestCount++] = req;
    saveRequests(list, requestCount);
    printf(">> Request submitted successfully. Please wait for the admin to review!\n");
}

void viewUnlockRequests(Account list[], int accountCount) 
{
    Request requests[100];
    Account accounts[100];

    int requestCount = loadRequests(requests);

    if (requestCount == 0) {
        printf("No unlock requests found.\n");
        return;
    }

    printf("\n===== LIST OF REQUESTS =====\n");

    for (int i = 0; i < requestCount; i++) {
        printf("%s - %s\n",
               requests[i].mssv,
               requests[i].name);
    }

    char mssv[20];
    int valid;

    do {
        valid = 0;
        
        printf("=============================================");
        printf("\nEnter the student ID you want to unlock: ");
        scanf("%19s", mssv);

        for (int i = 0; i < requestCount; i++) {
            if (strcmp(mssv, requests[i].mssv) == 0) {
                valid = 1;
                break;
            }
        }

        if (!valid) {
            printf("Invalid student ID!\n");
        }

    } while (!valid);

    if (confirmAction("Approve this request?: ")){
    for (int i = 0; i < accountCount; i++) {

        if (strcmp(accounts[i].studentid, mssv) == 0) {

            accounts[i].isLocked = 0;
            accounts[i].failCount = 0;

            printf("\nAccount unlocked for %s - %s\n",
                   accounts[i].username,
                   accounts[i].studentid);
            break;
        }
    }
    saveAccounts(accounts, accountCount);

    deleteUnlockRequest(requests, requestCount, mssv);
    } else 
        {
            printf(">> Request denied. Account remains locked!\n");
        }
}

void deleteUnlockRequest(Request requests[], int requestCount, char mssv[]) 
{
    int deleteIndex = -1;

    for (int i = 0; i < requestCount; i++) {

        if (strcmp(requests[i].mssv, mssv) == 0) {
            deleteIndex = i;
            break;
        }
    }

    for (int i = deleteIndex; i < requestCount - 1; i++) {
        requests[i] = requests[i + 1];
    }

    requestCount--;

    saveRequests(requests, requestCount);

    printf("The request has been deleted.\n");
}