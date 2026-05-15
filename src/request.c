#include <stdio.h>
#include <string.h>
#include "../includes/request.h"
#include "../includes/auth.h"
#include "../includes/utils.h"
#include "../includes/fileio.h"


int loadRequests(Request list[]) {
    FILE *f = fopen("data/request.dat", "rb");
    if (f == NULL) return 0;
    int count = 0;
    while (fread(&list[count], sizeof(Request), 1, f)) count++;
    fclose(f);
    return count;
}

void saveRequests(Request list[], int count) {
    FILE *f = fopen("data/request.dat", "wb");
    if (f == NULL) return;
    fwrite(list, sizeof(Request), count, f);
    fclose(f);
}

// ===================== MEMBER =====================
void sendUnlockRequest(Account *currentAcc) {
    if (currentAcc->isLocked == 0) {
        printf(">> Your account is not locked!\n");
        return;
    }

    // Kiểm tra đã gửi yêu cầu chưa
    Request list[MAX_REQUESTS];
    int count = loadRequests(list);
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].studentid, currentAcc->studentid) == 0 && list[i].status == 0) {
            printf(">> You have already submitted a request, please wait for the admin to review!\n");
            return;
        }
    }

    char reason[200];
    printf("Enter the reason for the unlock request: ");
    scanf(" %[^\n]", reason);
    clearBuffer();

    if (!confirmAction("Confirm sending the unlock request?")) {
        printf(">> Request cancelled.\n");
        return;
    }

    Request req;
    strcpy(req.studentid, currentAcc->studentid);
    strcpy(req.reason, reason);
    req.status = 0;

    list[count++] = req;
    saveRequests(list, count);
    printf(">> Request submitted successfully. Please wait for the admin to review!\n");
}

// ===================== ADMIN =====================
void viewUnlockRequests(Account list[], int accountCount) {
    Request reqList[MAX_REQUESTS];
    int count = loadRequests(reqList);

    // Lọc và hiển thị danh sách chờ duyệt
    int pendingIndex[MAX_REQUESTS];
    int pendingCount = 0;

    printf("\n--- UNLOCK REQUESTS ---\n");
    for (int i = 0; i < count; i++) {
        if (reqList[i].status == 0) {
            printf("[%d] Student ID: %10s | Reason: %s\n",
                pendingCount + 1,
                reqList[i].studentid,
                reqList[i].reason);
            pendingIndex[pendingCount++] = i;
        }
    }

    if (pendingCount == 0) {
        printf(">> There are no requests pending review.\n");
        return;
    }

    printf("\nEnter the number of the request you want to process (1 - %d): ", pendingCount);
    int choice = validInput(1, pendingCount);
    int reqIdx = pendingIndex[choice - 1];

    int accIdx = -1;
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(list[i].studentid, reqList[reqIdx].studentid) == 0) {
            accIdx = i; // Tìm thấy index thực sự trong mảng Account
            break;
        }
    }

    if (accIdx == -1) {
        printf(">> Error: Account for Student ID %s no longer exists!\n", reqList[reqIdx].studentid);
        return;
    }

    printf("\nRequest for Student ID: %s\n", list[accIdx].studentid);
    printf("Reason: %s\n", reqList[reqIdx].reason);

    if (confirmAction("Approve this request?")) {
        // reqList[reqIdx].status = 1;       
                list[accIdx].isLocked = 0; // mở
                list[accIdx].failCount = 0; // reset lần nhập lại
                reqList[reqIdx].status = 1; // duyệt
                printf(">> Request approved and account unlocked %s!\n", list[accIdx].studentid);
        reqList[reqIdx].status = 1;
    } else {
        list[accIdx].isLocked = 1; //bị khóa
        reqList[reqIdx].status = 2; // bị từ chối
        printf(">> Request denied. Account %s remains locked!\n", list[accIdx].studentid);
    }
    saveAccounts(list, accountCount);
    saveRequests(reqList, count);
}



