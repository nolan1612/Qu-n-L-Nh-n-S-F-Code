#include <stdio.h>
#include <string.h>
#include "../includes/request.h"
#include "../includes/auth.h"
#include "../includes/utils.h"


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
    int idx = pendingIndex[choice - 1];

    printf("\nRequest for Student ID: %s\n", reqList[idx].studentid);
    printf("Reason: %s\n", reqList[idx].reason);

    if (confirmAction("Approve this request?")) {
        reqList[idx].status = 1;
        for (int i = 0; i < accountCount; i++) {
            if (strcmp(list[i].studentid, reqList[idx].studentid) == 0) {
                list[i].isLocked = 0;
                list[i].failCount = 0;
                printf(">> Request approved and account unlocked %s!\n", list[i].studentid);
                break;
            }
        }
    } else {
        reqList[idx].status = 2;
        printf(">> Request denied for student %s!\n", reqList[idx].studentid);
    }

    saveRequests(reqList, count);
}

