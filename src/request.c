#include <stdio.h>
#include <string.h>
#include "../includes/request.h"
#include "../includes/auth.h"
#include "../includes/utils.h"
#include "../includes/fileio.h"



// // ===================== MEMBER =====================
void sendUnlockRequest(Account *currentAcc, Account list[], int accountCount) {
    if (currentAcc == NULL) {
        printf(">> Error: Invalid account!\n");
        return;
    }
    if (currentAcc->isLocked == 0) {
        printf(">> Your account is not locked!\n");
        return;
    }

    // Kiểm tra đã gửi yêu cầu chưa
    // Request list[MAX_REQUESTS];
    // int count = loadRequests(list);
    // for (int i = 0; i < count; i++) {
    //     if (strcmp(list[i].studentid, currentAcc->studentid) == 0 && list[i].status == 0) {
    //         printf(">> You have already submitted a request, please wait for the admin to review!\n");
    //         return;
    //     }
    // }
    if (currentAcc->isLocked == 2) {
        printf(">> You have already submitted a request, please wait for admin!\n");
        return;
    }  
    
    char reason[200];
    printf("Enter the reason for the unlock request: ");
    clearBuffer();
    scanf(" %[^\n]", reason);

    if (!confirmAction("Confirm sending the unlock request?")) {
        printf(">> Request cancelled.\n");
        return;
    }

    // Request req;
    // strcpy(req.studentid, currentAcc->studentid);
    // strcpy(req.reason, reason);
    // req.status = 0;

    // list[count++] = req;

    strcpy(currentAcc->reason, reason);
    currentAcc->isLocked = 2;

    saveAccounts(list, accountCount);

    printf("\n>> Request submitted successfully!\n");
    printf(">> Your reason: %s\n", reason);
    printf(">> Please wait for the admin to review.\n");

}

// ===================== ADMIN =====================
void viewUnlockRequests(Account list[], int accountCount) {
    // Request reqList[MAX_REQUESTS];
    // int count = loadRequests(reqList);

    // Lọc và hiển thị danh sách chờ duyệt
    int pendingIndex[MAX_ACCOUNTS];
    int pendingCount = 0;

    printf("\n--- UNLOCK REQUESTS ---\n");
    for (int i = 0; i < accountCount; i++) {
        if (list[i].isLocked == 2) {
            printf("[%d] Student ID : %s\n", pendingCount + 1, list[i].studentid);
            printf("    Username  : %s\n", list[i].username);
            printf("    Reason    : %s\n", list[i].reason);
            //     pendingCount + 1,
            //     list[i].studentid,
            //     list[i].reason);
            pendingIndex[pendingCount++] = i;
        }
    }

    if (pendingCount == 0) {
        printf(">> There are no requests pending review.\n");
        return;
    }

    printf("\nEnter the number of the request you want to process (1 - %d): ", pendingCount);
    int choice = validInput(1, pendingCount);
    int idx = pendingIndex[choice - 1];

    printf("\nStudent ID : %s\n", list[idx].studentid);
    printf("Username   : %s\n", list[idx].username);
printf("Reason     : %s\n", list[idx].reason); 
    // int accIdx = -1;
    // for (int i = 0; i < accountCount; i++) {
    //     if (strcmp(list[i].studentid, list[idx].studentid) == 0) {
    //         accIdx = i; // Tìm thấy index thực sự trong mảng Account
    //         break;
    //     }
    // }

    // if (accIdx == -1) {
    //     printf(">> Error: Account for Student ID %s no longer exists!\n", list[idx].studentid);
    //     return;
    // }

    // printf("\nRequest for Student ID: %s\n", list[accIdx].studentid);
    // printf("Reason: %s\n", list[idx].reason);

    if (confirmAction("Approve this request?")) {
        // reqList[reqIdx].status = 1;       
                list[idx].isLocked = 0; // mở
                list[idx].failCount = 0; // reset lần nhập lại
                // reqList[reqIdx].status = 1; // duyệt
                // memset(list[idx].reason, 0, sizeof(list[idx].reason));
                printf(">> Request approved and account unlocked %s!\n", list[idx].studentid);
        // reqList[reqIdx].status = 1;
    } else {
        list[idx].isLocked = 1; //bị khóa
        // reqList[reqIdx].status = 2; // bị từ chối
        //  memset(list[idx].reason, 0, sizeof(list[idx].reason));
        printf(">> Request denied. Account %s remains locked!\n", list[idx].studentid);
    }
    saveAccounts(list, accountCount);
    // saveRequests(reqList, count);
}



