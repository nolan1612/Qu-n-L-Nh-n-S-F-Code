// Code for assigning tasks to staff members and viewing staff lists
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/auth.h"
#include "../includes/event.h"
#include "../includes/menu.h"
#include "../includes/fileio.h"
#include "../includes/report.h"
#include "../includes/staff.h"
#include "../includes/utils.h"

void toUpperCaseStr(char str[]) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
    }
}

void addStaffToEvent(Event events[], int count, Account list[], int accountCount) {
    char searchId[10];
    int foundIndex = -1;
    int attempts = 3;

    printf("\n--- ADD STAFF TO EVENT ---\n");

    while (attempts > 0) {
        printf("Enter event ID (Ex: EV000001): ");
        scanf(" %[^\n]", searchId);

        for (int i = 0; i < count; i++) {
            if (strcmp(events[i].eventId, searchId) == 0) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            break;
        }

        attempts--;
        if (attempts > 0) {
            printf(">> Error: Cannot find event with ID %s! You have %d attempt(s) left.\n", searchId, attempts);
        } else {
            printf(">> Error: Cannot find event with ID %s! Maximum attempts reached. Exiting...\n", searchId);
            return;
        }
    }

    if (events[foundIndex].status == 2) {
        printf(">> Error: Event has ENDED, cannot add staff!\n");
        return;
    }

    if (events[foundIndex].staffCount >= 30) {
        printf(">> Error: Event has reached the maximum of 30 staff members!\n");
        return;
    }

    char mssvInput[50];
    int accountIndex = -1;
    attempts = 3;

    while (attempts > 0) {
        int existedInEvent = 0;
        accountIndex = -1;

        printf("Enter staff student ID: ");
        scanf(" %[^\n]", mssvInput);

        toUpperCaseStr(mssvInput);
        for (int i = 0; i < events[foundIndex].staffCount; i++) {
            if (strcmp(events[foundIndex].staffList[i].studentId, mssvInput) == 0) {
                existedInEvent = 1;
                break;
            }
        }
        for (int i = 0; i < accountCount; i++) {
            if (strcmp(list[i].studentid, mssvInput) == 0) {
                accountIndex = i;
                break;
            }
        }
        if (existedInEvent) {
            attempts--;
            if (attempts > 0) {
                printf(">> Error: Staff member %s already exists in the event! You have %d attempt(s) left.\n", mssvInput, attempts);
            } else {
                printf(">> Error: Staff member %s already exists in the event! Maximum attempts reached. Exiting...\n", mssvInput);
                return;
            }
        } else if (accountIndex == -1) {
            attempts--;
            if (attempts > 0) {
                printf(">> Error: Student ID %s does not exist in the system! You have %d attempt(s) left.\n", mssvInput, attempts);
            } else {
                printf(">> Error: Student ID %s does not exist in the system! Maximum attempts reached. Exiting...\n", mssvInput);
                return;
            }
        } else {
            break; 
        }
    }

    int role;
    do {
        printf("Enter role (0 = Leader, 1 = Member, 2 = Support): ");
        if (scanf("%d", &role) != 1) {
            clearBuffer();
            role = -1;
        }
        if (role < 0 || role > 2) {
            printf(">> Error: Role must be 0, 1, or 2.\n");
        }
    } while (role < 0 || role > 2);

    char desc[100];
    printf("Enter task description: ");
    scanf(" %[^\n]", desc);

    int sCount = events[foundIndex].staffCount;

    strcpy(events[foundIndex].staffList[sCount].studentId, mssvInput);
    strcpy(events[foundIndex].staffList[sCount].studentName, list[accountIndex].username);
    events[foundIndex].staffList[sCount].role = role;
    strcpy(events[foundIndex].staffList[sCount].description, desc);

    events[foundIndex].staffCount++;

    printf(">> Success: Added staff member %s to event %s.\n", mssvInput, searchId);

    saveEvents(events, count);
}

void editStaffRole(Event events[], int count) {
    char searchId[10];
    int foundIndex = -1;
    int attempts = 3;

    printf("\n--- EDIT STAFF INFORMATION IN EVENT ---\n");

    while (attempts > 0) {
        printf("Enter event ID (Ex: EV000001): ");
        scanf(" %[^\n]", searchId);

        for (int i = 0; i < count; i++) {
            if (strcmp(events[i].eventId, searchId) == 0) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            break;
        }

        attempts--;
        if (attempts > 0) {
            printf(">> Error: Cannot find event with ID %s! You have %d attempt(s) left.\n", searchId, attempts);
        } else {
            printf(">> Error: Cannot find event with ID %s! Maximum attempts reached. Exiting...\n", searchId);
            return;
        }
    }

    if (events[foundIndex].status == 1) {
        printf(">> Error: Cannot edit staff while the event is ongoing!\n");
        return;
    }

    if (events[foundIndex].staffCount == 0) {
        printf(">> Error: The event has no staff members yet!\n");
        return;
    }

    char mssvInput[50];
    int staffIndex = -1;

    attempts = 3;

    while (attempts > 0) {
        printf("Enter student ID of the staff member to edit: ");
        scanf(" %[^\n]", mssvInput);

        toUpperCaseStr(mssvInput);

        for (int i = 0; i < events[foundIndex].staffCount; i++) {
            if (strcmp(events[foundIndex].staffList[i].studentId, mssvInput) == 0) {
                staffIndex = i;
                break;
            }
        }

        if (staffIndex != -1) {
            break;
        }

        attempts--;
        if (attempts > 0) {
            printf(">> Error: Cannot find staff member %s in the event! You have %d attempt(s) left.\n", mssvInput, attempts);
        } else {
            printf(">> Error: Cannot find staff member %s in the event! Maximum attempts reached. Exiting...\n", mssvInput);
            return;
        }
    }

    int newRole;
    do {
        printf("Enter new role (0 = Leader, 1 = Member, 2 = Support): ");
        if (scanf("%d", &newRole) != 1) {
            clearBuffer();
            newRole = -1;
        }
        if (newRole < 0 || newRole > 2) {
            printf(">> Error: Role must be 0, 1, or 2.\n");
        }
    } while (newRole < 0 || newRole > 2);
    events[foundIndex].staffList[staffIndex].role = newRole;

    printf("Enter new task description: ");
    scanf(" %[^\n]", events[foundIndex].staffList[staffIndex].description);

    printf(">> Success: Updated staff member %s in event %s.\n", mssvInput, searchId);
    saveEvents(events, count);
}

void removeStaffFromEvent(Event events[], int count) {
    char searchId[10];
    int foundIndex = -1;
    int attempts = 3;

    printf("\n--- REMOVE STAFF FROM EVENT ---\n");

    while (attempts > 0) {
        printf("Enter event ID (Ex: EV000001): ");
        scanf(" %[^\n]", searchId);

        for (int i = 0; i < count; i++) {
            if (strcmp(events[i].eventId, searchId) == 0) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            break;
        }

        attempts--;
        if (attempts > 0) {
            printf(">> Error: Cannot find event with ID %s! You have %d attempt(s) left.\n", searchId, attempts);
        } else {
            printf(">> Error: Cannot find event with ID %s! Maximum attempts reached. Exiting...\n", searchId);
            return;
        }
    }

    if (events[foundIndex].status == 1) {
        printf(">> Error: Cannot remove staff while the event is ongoing!\n");
        return;
    }

    if (events[foundIndex].staffCount == 0) {
        printf(">> Error: The event has no staff members yet!\n");
        return;
    }

    printf("Current staff list:\n");

    for (int i = 0; i < events[foundIndex].staffCount; i++) {
        printf("- Student ID: %s | Role: %d | Task: %s\n",
               events[foundIndex].staffList[i].studentId,
               events[foundIndex].staffList[i].role,
               events[foundIndex].staffList[i].description);
    }

    char mssvInput[50];
    int staffIndex = -1;

    attempts = 3;

    while (attempts > 0) {
        printf("Enter student ID of the staff member to remove: ");
        scanf(" %[^\n]", mssvInput);

        toUpperCaseStr(mssvInput);

        for (int i = 0; i < events[foundIndex].staffCount; i++) {
            if (strcmp(events[foundIndex].staffList[i].studentId, mssvInput) == 0) {
                staffIndex = i;
                break;
            }
        }

        if (staffIndex != -1) {
            break;
        }

        attempts--;
        if (attempts > 0) {
            printf(">> Error: Cannot find staff member %s in the event! You have %d attempt(s) left.\n", mssvInput, attempts);
        } else {
            printf(">> Error: Cannot find staff member %s in the event! Maximum attempts reached. Exiting...\n", mssvInput);
            return;
        }
    }

    char confirm;

    printf("Are you sure you want to remove staff member %s from this event? (y/n): ", mssvInput);
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        for (int i = staffIndex; i < events[foundIndex].staffCount - 1; i++) {
            events[foundIndex].staffList[i] = events[foundIndex].staffList[i + 1];
        }

        events[foundIndex].staffCount--;

        printf(">> Success: Removed staff member %s from event %s.\n", mssvInput, searchId);
    } else {
        printf(">> Delete operation cancelled.\n");
    }

    saveEvents(events, count);
}

void eventJoin(Event events[], int eventCount, Account *currentAcc) {
    printf("\n--- EVENT CAN PARTICIPATE IN ---\n");
    if (eventCount == 0) {
        printf(">> Notice: No events in the system.\n");
        return;
    }
    int availableCount = 0;
    printf("%-15s | %-30s | %-10s | %-15s\n", "Event ID", "Event Name", "Staff", "Pending Requests");
    printf("-------------------------------------------------------------------------------\n");   
    for (int i = 0; i < eventCount; i++) {
        if (events[i].status != 2 && events[i].staffCount < 30) {
            int alreadyJoined = 0;
            for (int j = 0; j < events[i].staffCount; j++) {
                if (strcmp(events[i].staffList[j].studentId, currentAcc->studentid) == 0) {
                    alreadyJoined = 1; 
                    break;
                }
            }
            int alreadyRequested = 0;
            for (int j = 0; j < events[i].requestCount; j++) {
                if (strcmp(events[i].requestList[j], currentAcc->studentid) == 0) {
                    alreadyRequested = 1; 
                    break;
                }
            }
            if (!alreadyJoined && !alreadyRequested) {
                printf("%-15s | %-30s | %d/30      | %d\n", 
                       events[i].eventId, events[i].name, events[i].staffCount, events[i].requestCount);
                availableCount++;
            }
        }
    }

    if (availableCount == 0) {
        printf(">> Notice: There are no new events available for you to join right now.\n");
        return;
    }

    char targetId[15];
    printf("\nEnter Event ID you want to join (or '0' to cancel): ");
    scanf(" %[^\n]", targetId);

    if (strcmp(targetId, "0") == 0) {
        printf(">> Cancelled.\n");
        return;
    }

    int foundIndex = -1;
    for (int i = 0; i < eventCount; i++) {
        if (strcmp(events[i].eventId, targetId) == 0) {
            foundIndex = i; 
            break;
        }
    }

    if (foundIndex == -1) {
        printf(">> Error: Event ID not found!\n");
        return;
    }

    if (events[foundIndex].requestCount >= 30) {
        printf(">> Error: Request queue is full for this event!\n");
        return;
    }
    strcpy(events[foundIndex].requestList[events[foundIndex].requestCount], currentAcc->studentid);
    events[foundIndex].requestCount++; 
    
    saveEvents(events, eventCount);
    printf(">> Success: Join request sent for event %s. Please wait for admin approval.\n", targetId);
}

void approveJoinRequests(Event events[], int count, Account list[], int accountCount) {
    printf("\n--- APPROVE JOIN REQUESTS ---\n");

    int hasRequests = 0;
    printf("%-10s | %-25s | %-15s\n", "Event ID", "Event Name", "Pending Student");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < events[i].requestCount; j++) {
            printf("%-10s | %-25s | %-15s\n", 
                   events[i].eventId, 
                   events[i].name, 
                   events[i].requestList[j]);
            hasRequests = 1;
        }
    }

    if (!hasRequests) {
        printf(">> Notice: No pending join requests at the moment.\n");
        return;
    }

    char searchEv[15], searchStu[50];
    printf("\nEnter Event ID to process: ");
    scanf(" %[^\n]", searchEv);

    int evIndex = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(events[i].eventId, searchEv) == 0) {
            evIndex = i; 
            break;
        }
    }
    if (evIndex == -1) {
        printf(">> Error: Event not found.\n");
        return;
    }

    if (events[evIndex].requestCount == 0) {
        printf(">> Error: No pending requests for this event.\n");
        return;
    }

    printf("Enter Student ID to approve/reject: ");
    scanf(" %[^\n]", searchStu);
    for (int i = 0; searchStu[i] != '\0'; i++) {
        if (searchStu[i] >= 'a' && searchStu[i] <= 'z') {
            searchStu[i] = searchStu[i] - 32;
        }
    }

    int reqIndex = -1;
    for (int j = 0; j < events[evIndex].requestCount; j++) {
        if (strcmp(events[evIndex].requestList[j], searchStu) == 0) {
            reqIndex = j; 
            break;
        }
    }

    if (reqIndex == -1) {
        printf(">> Error: No pending request found for this student in this event.\n");
        return;
    }

    if (events[evIndex].staffCount >= 30) {
        printf(">> Error: Cannot approve. Event has reached max capacity (30 staff)!\n");
        return;
    }

    char decision;
    printf("Approve this student? (y = Yes/Approve, n = No/Reject): ");
    scanf(" %c", &decision);

    if (decision == 'y' || decision == 'Y') {
        int newRole;
        do {
            printf("Assign role (1 = Member, 2 = Support): ");
            scanf("%d", &newRole);
            if (newRole != 1 && newRole != 2) {
                printf(">> Error: Role must be 1 or 2.\n");
            }
        } while (newRole != 1 && newRole != 2);

        char newDesc[100];
        printf("Enter task description for this student: ");
        scanf(" %[^\n]", newDesc);
        int sCount = events[evIndex].staffCount;
        char studentName[50];
        strcpy(studentName, searchStu);
        for (int i = 0; i < accountCount; i++) {
            if (strcmp(list[i].studentid, searchStu) == 0) {
                strcpy(studentName, list[i].username);
                break;
            }
        }
        strcpy(events[evIndex].staffList[sCount].studentId, searchStu);
        strcpy(events[evIndex].staffList[sCount].studentName, studentName);
        events[evIndex].staffList[sCount].role = newRole;
        strcpy(events[evIndex].staffList[sCount].description, newDesc);
        
        events[evIndex].staffCount++;

        printf(">> Success: Student %s has been APPROVED for event %s.\n", searchStu, searchEv);
    } else {
        printf(">> Success: Request from %s has been REJECTED.\n", searchStu);
    }
    for (int k = reqIndex; k < events[evIndex].requestCount - 1; k++) {
        strcpy(events[evIndex].requestList[k], events[evIndex].requestList[k + 1]);
    }
    events[evIndex].requestCount--;
    saveEvents(events, count);
}
