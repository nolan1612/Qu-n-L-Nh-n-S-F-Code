#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../includes/auth.h"
#include "../includes/event.h"
#include "../includes/menu.h"
#include "../includes/fileio.h"
#include "../includes/report.h"
#include "../includes/staff.h"
#include "../includes/utils.h"

int checkOverlap(Event events[], int count, const char* newStart, const char* newEnd, const char* ignoreEventId) {
    for (int i = 0; i < count; i++) {
        if (ignoreEventId != NULL && strcmp(events[i].eventId, ignoreEventId) == 0) {
            continue;
        }
        if (strcmp(newStart, events[i].endDate) <= 0 && strcmp(events[i].startDate, newEnd) <= 0) {
            return 1;
        }
    }
    return 0;
}

void generateEventId(char *dest, Event events[], int count) {
    int maxId = 0;
    for (int i = 0; i < count; i++) {
        int currentId;
        if (sscanf(events[i].eventId, "EV%d", &currentId) == 1) {
            if (currentId > maxId) {
                maxId = currentId;
            }
        }
    }
    sprintf(dest, "EV%06d", maxId + 1);
}

void createEvent(Event events[], int *count) {
    if (*count >= MAX_EVENTS) {
        printf("System is full, cannot create more events!\n");
        return;
    }

    Event newEv;
    generateEventId(newEv.eventId, events, *count); 
    
    printf("Enter event name: ");
    scanf(" %[^\n]", newEv.name);
    
    printf("Enter event description: ");
    scanf(" %[^\n]", newEv.description);
    
    printf("Enter event location: ");
    scanf(" %[^\n]", newEv.location);

    while (1) {
        printf("Enter start date (YYYY-MM-DD or YYYY/MM/DD): ");
        inputValidFormatDate(newEv.startDate);

        printf("Enter end date (YYYY-MM-DD or YYYY/MM/DD): ");
        inputValidFormatDate(newEv.endDate);
        
        char currentDate[15];
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(currentDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

        if (strcmp(newEv.startDate, currentDate) < 0) {
            printf("\033[1;31m>> Error: Start date cannot be in the past (Current date is %s)!\033[0m\n", currentDate);
            continue;
        }

        if (strcmp(newEv.endDate, newEv.startDate) < 0) {
            printf("\033[1;31m>> Error: End date must be after or equal to start date!\033[0m\n");
            continue;
        }

        int days = getDaysDifference(newEv.startDate, newEv.endDate) + 1;
        if (days < 1) {
            printf("\033[1;31m>> Error: Event must last at least 1 days (Currently %d days)!\033[0m\n", days);
            continue;
        }
        if (days > 100) {
            printf("\033[1;31m>> Error: Event take to long (Currently %d days)!\033[0m\n", days);
            continue;
        }
        if (checkOverlap(events, *count, newEv.startDate, newEv.endDate, NULL)) {
            printf("\033[1;33m>> Error: This time overlaps with another existing event!\033[0m\n");
            continue;
        }

        break;
    }
    
    newEv.status = 0;
    newEv.staffCount = 0;
    newEv.requestCount = 0;
    
    events[*count] = newEv;
    (*count)++;
    saveEvents(events, *count);
    printf(">> Success: Created event %s\n", newEv.eventId);
}

void editEvent(Event events[], int count) {
    char searchId[10];
    int foundIndex = -1;
    int attempts = 3;
    char tempInput[256];

    printf("\n--- EDIT EVENT INFORMATION ---\n");
    while (attempts > 0) {
        printf("Enter event ID to edit (e.g., EV000001): ");
        scanf(" %9[^\n]", searchId);
        while (getchar() != '\n'); 

        foundIndex = -1;
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
        printf(">> Error: Cannot edit a finished event!\n");
        return;
    }

    if (events[foundIndex].status == 1) {
        char confirm;
        printf("!!! WARNING: This event is ONGOING. Are you sure you want to edit? (y/n): ");
        scanf(" %c", &confirm);
        while (getchar() != '\n');
        if (confirm != 'y' && confirm != 'Y') {
            printf(">> Edit operation cancelled.\n");
            return;
        }
    }

    printf("\nNote: Enter '0' to keep the current information.\n");

    printf("Current name: %s\n", events[foundIndex].name);
    printf("Enter new name: ");
    scanf(" %199[^\n]", tempInput); 
    while (getchar() != '\n');
    if (strcmp(tempInput, "0") != 0) {
        strcpy(events[foundIndex].name, tempInput);
    }

    printf("Current description: %s\n", events[foundIndex].description);
    printf("Enter new description: ");
    scanf(" %50[^\n]", tempInput);
    while (getchar() != '\n');
    if (strcmp(tempInput, "0") != 0) {
        strcpy(events[foundIndex].description, tempInput);
    }

    printf("Current location: %s\n", events[foundIndex].location);
    printf("Enter new location: ");
    scanf(" %99[^\n]", tempInput);
    while (getchar() != '\n');
    if (strcmp(tempInput, "0") != 0) {
        strcpy(events[foundIndex].location, tempInput);
    }

    while (1) {
        char tempStart[15], tempEnd[15];
        
        printf("Current start date: %s\n", events[foundIndex].startDate);
        printf("Enter new start date (YYYY-MM-DD or YYYY/MM/DD): ");
        scanf(" %14[^\n]", tempStart);
        while (getchar() != '\n');

        printf("Current end date: %s\n", events[foundIndex].endDate);
        printf("Enter new end date (YYYY-MM-DD or YYYY/MM/DD): ");
        scanf(" %14[^\n]", tempEnd);
        while (getchar() != '\n');
        
        for(int k = 0; tempStart[k] != '\0'; k++) if(tempStart[k] == '/') tempStart[k] = '-';
        for(int k = 0; tempEnd[k] != '\0'; k++) if(tempEnd[k] == '/') tempEnd[k] = '-';

        char finalStart[15], finalEnd[15];
        strcpy(finalStart, (strcmp(tempStart, "0") == 0) ? events[foundIndex].startDate : tempStart);
        strcpy(finalEnd, (strcmp(tempEnd, "0") == 0) ? events[foundIndex].endDate : tempEnd);
        
        if (!isValidDateStr(finalStart) || !isValidDateStr(finalEnd)) {
            printf("\033[1;31m>> Error: Invalid date format or date does not exist!\033[0m\n");
            continue;
        }
        
        char currentDate[15];
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(currentDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        if (strcmp(tempStart, "0") != 0 && strcmp(finalStart, currentDate) < 0) {
            printf("\033[1;31m>> Error: New start date cannot be in the past (Current date is %s)!\033[0m\n", currentDate);
            continue;
        }

        if (strcmp(finalEnd, finalStart) < 0) {
            printf("\033[1;31m>> Error: End date must be after or equal to start date!\033[0m\n");
            continue;
        }

        int days = getDaysDifference(finalStart, finalEnd) + 1;
        if (days < 1) {
            printf("\033[1;31m>> Error: Event must last at least 1 days (Currently %d days)!\033[0m\n", days);
            continue;
        }
        if (days > 100) {
            printf("\033[1;31m>> Error: Event take to long (Currently %d days)!\033[0m\n", days);
            continue;
        }

        if (checkOverlap(events, count, finalStart, finalEnd, events[foundIndex].eventId)) {
            printf("\033[1;33m>> Error: New time overlaps with another event in the system!\033[0m\n");
            continue;
        }

        strcpy(events[foundIndex].startDate, finalStart);
        strcpy(events[foundIndex].endDate, finalEnd);
        break;
    }

    printf(">> Success: Updated information for event %s.\n", searchId);
    saveEvents(events, count);
}

void updateEventStatus(Event events[], int count) {
    char searchId[10];
    int foundIndex = -1;
    int attempts = 3;

    printf("\n--- UPDATE EVENT STATUS ---\n");
    while (attempts > 0) {
        printf("Enter event ID (e.g., EV000001): ");
        scanf(" %[^\n]", searchId);
        foundIndex = -1;
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

    int currentStatus = events[foundIndex].status;
    printf("Current status: ");
    if (currentStatus == 0) printf("Not started\n");
    else if (currentStatus == 1) printf("Ongoing\n");
    else printf("Finished\n");

    if (currentStatus == 2) {
        printf(">> Error: Event is FINISHED, status cannot be changed anymore!\n");
        return;
    }

    if (currentStatus == 0) {
        char confirm;
        printf("Do you want to change to [Ongoing]? (y/n): ");
        scanf(" %c", &confirm);
        
        if (confirm == 'y' || confirm == 'Y') {
            if (events[foundIndex].staffCount == 0) {
                char force;
                printf("!!! WARNING: Event has no staff. Continue anyway? (y/n): "); 
                scanf(" %c", &force);
                if (force != 'y' && force != 'Y') return;
            }
            events[foundIndex].status = 1;
            printf(">> Success: Event is now ONGOING.\n");
        }
    } 
    else if (currentStatus == 1) {
        char confirm;
        printf("Do you want to change to [Finished]? (y/n): ");
        scanf(" %c", &confirm);
        
        if (confirm == 'y' || confirm == 'Y') {
            events[foundIndex].status = 2;
            printf(">> Success: Event is now FINISHED.\n");
        }
    }
	saveEvents(events, count);
}

void deleteEvent(Event events[], int *count) {
    char searchId[10];
    int foundIndex = -1;
    int attempts = 3;
    printf("\n--- DELETE EVENT ---\n");
    while (attempts > 0) {
        printf("Enter event ID to delete (e.g., EV000001): ");
        scanf(" %[^\n]", searchId);
        foundIndex = -1;
        for (int i = 0; i < *count; i++) {
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
    printf("Event name: %s\n", events[foundIndex].name);
    printf("Number of staff currently in the event: %d\n", events[foundIndex].staffCount);
    if (events[foundIndex].status == 1) {
        printf(">> Error: Cannot delete an ongoing event!\n"); 
        return;
    }
    char confirm1;
    printf("!!! WARNING: Are you sure you want to delete this event? (y/n): ");
    scanf(" %c", &confirm1);
    if (confirm1 != 'y' && confirm1 != 'Y') {
        printf(">> Delete operation cancelled.\n");
        return;
    }
    char confirm2;
    printf("!!! 2ND CONFIRMATION: This action will permanently delete the event and all its staff. Continue? (y/n): ");
    scanf(" %c", &confirm2);
    if (confirm2 != 'y' && confirm2 != 'Y') {
        printf(">> Delete operation cancelled.\n");
        return;
    }

    for (int i = foundIndex; i < *count - 1; i++) {
        events[i] = events[i + 1];
    }
    (*count)--;
    printf(">> Success: Deleted event %s.\n", searchId);
    saveEvents(events, *count);
}

void displayAllEvents(Event events[], int count) {
    if (count == 0) {
        printf(">> Notice: Event list is empty.\n");
        return;
    }
    int filter;
    int attempts = 0;
    int max_attempts = 3;
    int valid_input = 0;
    while (attempts < max_attempts) {
        printf("\n--- FILTER EVENT LIST ---\n");
        printf("1. All\n");
        printf("2. Not started\n");
        printf("3. Ongoing\n");
        printf("4. Finished\n");
        printf("Select filter type (1-4): ");
        if (scanf("%d", &filter) != 1) {
            while (getchar() != '\n');
            printf(">> Error: Invalid input. Please enter a number.\n");
        } 
        else if (filter >= 1 && filter <= 4) {
            valid_input = 1;
            break;
        } 
        else {
            printf(">> Error: Choice out of range. Please select between 1 and 4.\n");
        }

        attempts++;
        if (attempts < max_attempts) {
            printf(">> You have %d attempt(s) left.\n", max_attempts - attempts);
        }
    }
    if (!valid_input) {
        printf(">> Notice: Too many invalid attempts. Returning to previous menu.\n");
        return;
    }
    int sortOrder;
    printf("\n--- SORT EVENTS BY START DATE ---\n");
    printf("1. Ascending (Oldest first)\n");
    printf("2. Descending (Newest first)\n");
    printf("3. Default (No sort)\n");
    printf("Select sort order (1-3): ");
    
    if (scanf("%d", &sortOrder) != 1) {
        while (getchar() != '\n');
        sortOrder = 3;
    }
    
    if (sortOrder == 1 || sortOrder == 2) {
        sortEventsByStartDate(events, count, sortOrder);
    }
    int targetStatus = filter - 2;

    printf("\n%-10s | %-20s | %-12s | %-12s | %-15s | %-5s | %-15s\n", 
           "ID", "Event Name", "Start Date", "End Date", "Location", "Staff", "Status");
    printf("------------------------------------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (filter == 1 || events[i].status == targetStatus) {
            char statusStr[20];
            if (events[i].status == 0) {
            	strcpy(statusStr, "Not started");
			}else if (events[i].status == 1) {
				strcpy(statusStr, "Ongoing");
			}
            else {
            	strcpy(statusStr, "Finished");
			}
            printf("%-10s | %-20s | %-12s | %-12s | %-15s | %-5d | %-15s\n", 
                   events[i].eventId, 
                   events[i].name, 
                   events[i].startDate, 
                   events[i].endDate, 
                   events[i].location, 
                   events[i].staffCount, 
                   statusStr);
            found = 1;
        }
    }

    if (!found) {
        printf(">> No events match the filter.\n");
    }
    printf("------------------------------------------------------------------------------------------------------------\n");
}

void viewMemberProfile(Account *currentAcc) {
    printf("\n=========================================\n");
    printf("           PERSONAL INFORMATION             \n");
    printf("=========================================\n");
    printf("%-12s: %s\n", "Full Name", currentAcc->username);
    printf("%-12s: %s\n", "Student ID", currentAcc->studentid);
    printf("%-12s: %s\n", "Email", currentAcc->email);
    printf("%-12s: %s\n", "Phone", currentAcc->phone);
    // printf("%-12s: %s\n", "Dept", currentAcc->dept);
    printf("%-12s: %s\n", "Role", (currentAcc->role >= 1) ? "Board of Directors (BOD)" : "Member");
    printf("=========================================\n");
    while (getchar() != '\n'); 
    getchar();
}

void viewMemberHistory(Event events[], int count) {
    char searchId[15];
    int found = 0;
    int attempts = 3;
    printf("\n--- VIEW MEMBER PARTICIPATION HISTORY ---\n");
    while (attempts > 0) {
        printf("Enter Student ID to search: ");
        scanf(" %[^\n]", searchId);
        found = 0;
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < events[i].staffCount; j++) {
                if (strcmp(events[i].staffList[j].studentId, searchId) == 0) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        if (found) {
            break;
        }

        attempts--;
        if (attempts > 0) {
            printf(">> Error: Cannot find any participation history for Student ID %s! You have %d attempt(s) left.\n", searchId, attempts);
        } else {
            printf(">> Error: Cannot find Student ID %s! Maximum attempts reached. Exiting...\n", searchId);
            return;
        }
    }
    printf("\n%-25s | %-10s | %-15s\n", "Event Name", "Role", "Status");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < events[i].staffCount; j++) {
            if (strcmp(events[i].staffList[j].studentId, searchId) == 0) {
                char roleStr[20];
                if (events[i].staffList[j].role == 0) {
                    strcpy(roleStr, "Leader");
                } else if (events[i].staffList[j].role == 1) {
                    strcpy(roleStr, "Member");
                } else if (events[i].staffList[j].role == 2) {
                    strcpy(roleStr, "Support");
                } else {
                    strcpy(roleStr, "Unknown");
                }
                char statusStr[20];
                if (events[i].status == 0) {
                    strcpy(statusStr, "Not started");
                } else if (events[i].status == 1) {
                    strcpy(statusStr, "Ongoing");
                } else if (events[i].status == 2) {
                    strcpy(statusStr, "Finished");
                } else {
                    strcpy(statusStr, "Unknown");
                }
                printf("%-25s | %-10s | %-15s\n", 
                       events[i].name, 
                       roleStr, 
                       statusStr);
                       
                break;
            }
        }
    }
    printf("----------------------------------------------------------\n");
}
void viewJoinedEventsList(Event events[], int count, Account *currentAcc) {
    int found = 0;
    printf("\n====================================================================================================\n");
    printf("                                   JOINED EVENTS LIST                                               \n");
    printf("====================================================================================================\n");
    printf("%-20s | %-22s | %-10s | %-20s | %-15s\n",
           "Event Name",
           "Event Date",
           "Role",
           "Task Description",
           "Status");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        if (events[i].status != 0 && events[i].status != 1) {
            continue;
        }
        for (int j = 0; j < events[i].staffCount; j++) {
            if (strcmp(events[i].staffList[j].studentId,
                       currentAcc->studentid) == 0) {
                char roleStr[20];
                char statusStr[20];
                switch (events[i].staffList[j].role) {
                    case 0:
                        strcpy(roleStr, "Leader");
                        break;
                    case 1:
                        strcpy(roleStr, "Member");
                        break;
                    case 2:
                        strcpy(roleStr, "Support");
                        break;
                    default:
                        strcpy(roleStr, "Unknown");
                }
                switch (events[i].status) {
                    case 0:
                        strcpy(statusStr, "Not started");
                        break;
                    case 1:
                        strcpy(statusStr, "Ongoing");
                        break;
                    default:
                        strcpy(statusStr, "Unknown");
                }
                printf("%-20s | %s to %s | %-10s | %-20s | %-15s\n",
                       events[i].name,
                       events[i].startDate,
                       events[i].endDate,
                       roleStr,
                       events[i].staffList[j].description,
                       statusStr);

                found = 1;
                break;
            }
        }
    }
    if (!found) {
        printf(">> Notice: No ongoing or upcoming events assigned to you.\n");
    }
    printf("----------------------------------------------------------------------------------------------------\n");
}
void viewJoinedEventDetails(Event events[], int count, Account *currentAcc) {
    char eventId[15];
    int foundIndex = -1;
    int joined = 0;
    int attempts = 3;
    printf("\n--- VIEW DETAILS OF JOINED EVENT ---\n");
    while (attempts > 0) {
        printf("Enter Event ID: ");
        scanf(" %[^\n]", eventId);
        foundIndex = -1;
        joined = 0;
        for (int i = 0; i < count; i++) {

            if (strcmp(events[i].eventId, eventId) == 0) {

                foundIndex = i;

                for (int j = 0; j < events[i].staffCount; j++) {

                    if (strcmp(events[i].staffList[j].studentId,
                               currentAcc->studentid) == 0) {

                        joined = 1;
                        break;
                    }
                }

                break;
            }
        }
        if (foundIndex == -1) {

            attempts--;

            if (attempts > 0) {

                printf(">> Error: Event ID not found! %d attempt(s) left.\n",
                       attempts);

            } else {

                printf(">> Error: Maximum attempts reached.\n");
                return;
            }
        }
        else if (!joined) {

            printf(">> Error: You are not assigned to this event.\n");
            return;
        }
        else {
            break;
        }
    }
    Event ev = events[foundIndex];
    char statusStr[20];
    switch (ev.status) {
        case 0:
            strcpy(statusStr, "Not started");
            break;
        case 1:
            strcpy(statusStr, "Ongoing");
            break;

        case 2:
            strcpy(statusStr, "Finished");
            break;
        default:
            strcpy(statusStr, "Unknown");
    }
    printf("\n============================================================\n");
    printf("                    EVENT DETAILS                           \n");
    printf("============================================================\n");
    printf("Event ID    : %s\n", ev.eventId);
    printf("Event Name  : %s\n", ev.name);
    printf("Description : %s\n", ev.description);
    printf("Location    : %s\n", ev.location);
    printf("Start Date  : %s\n", ev.startDate);
    printf("End Date    : %s\n", ev.endDate);
    printf("Status      : %s\n", statusStr);
    printf("\n==================== STAFF LIST ============================\n");
    printf("%-15s | %-15s\n", "Student ID", "Role");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < ev.staffCount; i++) {
        char roleStr[20];
        switch (ev.staffList[i].role) {
            case 0:
                strcpy(roleStr, "Leader");
                break;
            case 1:
                strcpy(roleStr, "Member");
                break;
            case 2:
                strcpy(roleStr, "Support");
                break;
            default:
                strcpy(roleStr, "Unknown");
        }
        printf("%-15s | %-15s\n",
               ev.staffList[i].studentId,
               roleStr);
    }
    printf("============================================================\n");
}
void viewMyParticipationHistory(Event events[], int count, Account *currentAcc) {
    static int historyIndexes[MAX_EVENTS];
    int historyCount = 0;
    for (int i = 0; i < count; i++) {
        if (events[i].status != 2) {
            continue;
        }
        for (int j = 0; j < events[i].staffCount; j++) {
            if (strcmp(events[i].staffList[j].studentId,
                       currentAcc->studentid) == 0) {
                historyIndexes[historyCount++] = i;
                break;
            }
        }
    }
    for (int i = 0; i < historyCount - 1; i++) {
        for (int j = i + 1; j < historyCount; j++) {
            if (strcmp(events[historyIndexes[i]].endDate,
                       events[historyIndexes[j]].endDate) < 0) {
                int temp = historyIndexes[i];
                historyIndexes[i] = historyIndexes[j];
                historyIndexes[j] = temp;
            }
        }
    }
    printf("\n====================================================================================\n");
    printf("                           PARTICIPATION HISTORY                                    \n");
    printf("====================================================================================\n");
    printf("%-25s | %-22s | %-10s | %-15s\n",
           "Event Name",
           "Event Date",
           "Role",
           "Status");
    printf("------------------------------------------------------------------------------------\n");
    if (historyCount == 0) {
        printf(">> Notice: No participation history found.\n");
    }
    for (int i = 0; i < historyCount; i++) {
        Event *ev = &events[historyIndexes[i]];
        char roleStr[20];
        strcpy(roleStr, "Unknown");
        for (int j = 0; j < ev->staffCount; j++) {
            if (strcmp(ev->staffList[j].studentId,
                       currentAcc->studentid) == 0) {
                switch (ev->staffList[j].role) {
                    case 0:
                        strcpy(roleStr, "Leader");
                        break;
                    case 1:
                        strcpy(roleStr, "Member");
                        break;
                    case 2:
                        strcpy(roleStr, "Support");
                        break;
                    default:
                        strcpy(roleStr, "Unknown");
                }
                break;
            }
        }
        printf("%-25s | %s to %s | %-10s | %-15s\n",
               ev->name,
               ev->startDate,
               ev->endDate,
               roleStr,
               "Finished");
    }
    printf("------------------------------------------------------------------------------------\n");
}

void searchEventsByTimeRange(Event events[], int count) {
    char searchStartDate[11];
    char searchEndDate[11];

    printf("\n============================================================\n");
    printf("                SEARCH EVENTS BY TIME RANGE                 \n");
    printf("============================================================\n");

    if (count == 0) {
        printf(">> Notice: No events in the system.\n");
        return;
    }

    while (1) {
        printf("Enter start date (YYYY-MM-DD or YYYY/MM/DD): ");
        inputValidFormatDate(searchStartDate);

        printf("Enter end date (YYYY-MM-DD or YYYY/MM/DD): ");
        inputValidFormatDate(searchEndDate);

        if (strcmp(searchEndDate, searchStartDate) < 0) {
            printf("\033[1;31m>> Error: End date must be after or equal to start date!\033[0m\n");
            continue;
        }
        break;
    }

    printf("\n%-10s | %-20s | %-12s | %-12s | %-15s\n", 
           "ID", "Event Name", "Start Date", "End Date", "Status");
    printf("--------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(events[i].startDate, searchStartDate) >= 0 && 
            strcmp(events[i].startDate, searchEndDate) <= 0) {
            
            char statusStr[20];
            if (events[i].status == 0) strcpy(statusStr, "Not started");
            else if (events[i].status == 1) strcpy(statusStr, "Ongoing");
            else strcpy(statusStr, "Finished");

            printf("%-10s | %-20s | %-12s | %-12s | %-15s\n", 
                   events[i].eventId, 
                   events[i].name, 
                   events[i].startDate, 
                   events[i].endDate, 
                   statusStr);
            found = 1;
        }
    }

    if (!found) {
        printf(">> Notice: No events found starting within this time range.\n");
    }
    printf("--------------------------------------------------------------------------------\n");
}

void sortEventsByStartDate(Event events[], int count, int sortOrder) {
    for (int i = 0; i < count - 1; i++) {
        int targetIdx = i;
        for (int j = i + 1; j < count; j++) {
            int cmp = strcmp(events[j].startDate, events[targetIdx].startDate);
            
            if ((sortOrder == 1 && cmp < 0) || (sortOrder == 2 && cmp > 0)) {
                targetIdx = j;
            }
        }
        if (targetIdx != i) {
            Event temp = events[i];
            events[i] = events[targetIdx];
            events[targetIdx] = temp;
        }
    }
}


void searchEventsByNameOrId(Event events[], int count) {
    printf("\n============================================================\n");
    printf("                  SEARCH EVENTS BY NAME/ID                  \n");
    printf("============================================================\n");
    
    char searchInput[256] = ""; 
    
    if (count == 0) {
        printf(">> Notice: No events in the system.\n");
        return;
    }

    printf("Enter event name or ID to search: ");
    while(getchar() != '\n'); 
    if (scanf("%255[^\n]", searchInput) != 1) {
        printf(">> Notice: Invalid search input.\n");
        while(getchar() != '\n'); 
        return;
    }
    while(getchar() != '\n');

    int *matchedIndexes = malloc(sizeof(int) * count);
    int *scores = malloc(sizeof(int) * count);
    if (matchedIndexes == NULL || scores == NULL) {
        printf(">> Error: Not enough memory to search events.\n");
        free(matchedIndexes);
        free(scores);
        return;
    }
    int matchCount = 0;

    for (int i = 0; i < count; i++) {
        int score = getSearchScore(events[i].name, searchInput);

        char idCopy[50];
        strcpy(idCopy, events[i].eventId);
        toLowerCase(idCopy);

        char searchCopy[256];
        strcpy(searchCopy, searchInput);
        toLowerCase(searchCopy);

        if (strstr(idCopy, searchCopy) != NULL) {
            score += 10;
        }
        
        if (score > 0) {
            matchedIndexes[matchCount] = i;
            scores[matchCount] = score;
            matchCount++;
        }
    }
    
    for (int i = 0; i < matchCount - 1; i++) {
        for (int j = i + 1; j < matchCount; j++) {
            if (scores[j] > scores[i]) {
                int tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;
                int tempIdx = matchedIndexes[i];
                matchedIndexes[i] = matchedIndexes[j];
                matchedIndexes[j] = tempIdx;
            }
        }
    }
    
    if (matchCount == 0) {
        printf(">> Notice: No events matched your search.\n");
    } else {
        printf("\n%-10s | %-20s | %-12s | %-12s | %-15s\n", 
               "ID", "Event Name", "Start Date", "End Date", "Status");
        printf("--------------------------------------------------------------------------------\n");
        for (int i = 0; i < matchCount; i++) {
            int idx = matchedIndexes[i];
            char statusStr[20];
            if (events[idx].status == 0) strcpy(statusStr, "Not started");
            else if (events[idx].status == 1) strcpy(statusStr, "Ongoing");
            else strcpy(statusStr, "Finished");

            printf("%-10s | %-20s | %-12s | %-12s | %-15s\n", 
                   events[idx].eventId, 
                   events[idx].name, 
                   events[idx].startDate, 
                   events[idx].endDate, 
                   statusStr);
        }
        printf("--------------------------------------------------------------------------------\n");
    }
    free(matchedIndexes);
    free(scores);
}
