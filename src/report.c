#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/event.h"
#include "../includes/report.h"


int eventSearch(Event list[], int countEvent, char eIdorName[])
{   
    int Index = -1;

    for (int i = 0; i < countEvent; i++)
    {
        if (strcmp(list[i].eventId, eIdorName) == 0 || strcasecmp(list[i].name, eIdorName) == 0)
        {
            Index = i;

            printf("The event code: %s\n", list[i].eventId);
            printf("The event name: %s\n", list[i].name);

            if (list[i].status == 0)
            {
                printf("the event status: Not Start\n");
            } else if (list[i].status == 1)
                    {
                        printf("the event status: Ongoing\n");
                    } else if (list[i].status == 2)
                            {
                                printf("the event status: Complete\n");
                            } else 
                                {
                                    printf("the event status: Unknown\n");
                                }

            return Index;
        }
    }

    if (Index == -1)
    {
        printf("The event code or name you entered does not exist!\n");
    }   
    
    return Index;
}


void eventDetail(Event list[], int countEvent)
{
    char eId[20];
    printf("Enter the event code you want to view details for (e.g., EV000001) or the event name:");
    scanf(" %[^\n]", eId);

    int Index = eventSearch(list, countEvent, eId);

    if (Index != -1)
    {
        printf("|===============================|\n");
        printf("|       EVENT INFORMATION       |\n");
        printf("|===============================|\n");

        printf("Event Code: %s\n", list[Index].eventId);
        printf("Event Name: %s\n", list[Index].name);

        if (list[Index].description[0] == '\0') 
        {
            printf("Event Description: No description provided\n");
        } else 
            {
                printf("Event Description: %s\n", list[Index].description);
            }

        printf("Event Location: %s\n", list[Index].location);
        printf("Event Start Date: %s\n", list[Index].startDate);
        printf("Event End Date: %s\n", list[Index].endDate);

        const char* status[] = {"Not Start", "Ongoing", "Complete", "Unknown"};
        int r = list[Index].status;
        const char* statusEvent = (r >= 0 && r <= 2) ? status[r] : status[3];
        printf("the event status: %s\n", statusEvent);

        if (list[Index].staffCount == 0)
        {
            printf("No staff have been assigned to this event yet!\n");
        } else
        {   
            printf("+---------------------------+--------------+------------+----------------------------------+\n");
            printf("|        LIST OF STAFF FOR THE EVENT (Total: %d)                                         |\n", list[Index].staffCount);
            printf("+---------------------------+--------------+------------+----------------------------------+\n");
            // Căn lề cố định: Tên (25 ký tự), ID (12 ký tự), Chức vụ (10 ký tự), Nhiệm vụ (30 ký tự)
            printf("| %-25s | %-12s | %-10s | %-32s |\n", "Name", "ID", "Role", "Responsibilities");
            printf("+---------------------------+--------------+------------+----------------------------------+\n");

            const char* ROLE_NAMES[] = {"BCN", "Member", "Support", "Unassigned"};

            for (int i = 0; i < list[Index].staffCount; i++)
            {   
                // Tối ưu lấy Role Name
                int r = list[Index].staffList[i].role;
                const char* roleStr = (r >= 0 && r <= 2) ? ROLE_NAMES[r] : ROLE_NAMES[3];

                // Tối ưu kiểm tra chuỗi rỗng bằng ký tự '\0' thay vì dùng strlen
                const char* descStr = list[Index].staffList[i].description;
                if (descStr[0] == '\0') {
                    descStr = "No responsibilities assigned";
                }

                // In một dòng dữ liệu thẳng hàng tuyệt đối
                printf("| %-25s | %-12s | %-10s | %-32s |\n", 
                        list[Index].staffList[i].studentName,
                        list[Index].staffList[i].studentId,
                        roleStr,
                        descStr);
            }
            printf("+---------------------------+--------------+------------+----------------------------------+\n");
        }  
    } else 
        {
            printf("The event code or name you entered does not exist!\n");
        }
}

void createFile(Event list[], int countEvent)
{
    printf("Search name or ID of event you want to create report for: ");
    char eId[20];
    
    // [FIX]: Giới hạn nhập tối đa 19 ký tự để chống lỗi tràn bộ nhớ (Buffer Overflow)
    scanf(" %19[^\n]", eId);

    // Tìm kiếm sự kiện
    int Index = eventSearch(list, countEvent, eId);

    if (Index != -1)
    {
        CREATE_DIR("reports");

        char filename[100];
        snprintf(filename, sizeof(filename), "reports/%s_report.txt", list[Index].eventId);

        FILE *fptr = fopen(filename, "w");
        if (fptr == NULL)
        {
            printf("\033[1;31m>> Error:\033[0m Can not create report file at %s!\n", filename);
            return;
        }

        const char* status[] = {"Not Start", "Ongoing", "Complete", "Unknown"};
        int r = list[Index].status;
        const char* statusEvent = (r >= 0 && r <= 2) ? status[r] : status[3];

        fprintf(fptr, "Event Code: %s\n", list[Index].eventId);
        fprintf(fptr, "Event Name: %s\n", list[Index].name);

        if (list[Index].description[0] == '\0') 
        {
            fprintf(fptr, "Event Description: No description provided\n");
        } else 
            {
                fprintf(fptr, "Event Description: %s\n", list[Index].description);
            }

        fprintf(fptr, "Event Location: %s\n", list[Index].location);
        fprintf(fptr, "Event Start Date: %s\n", list[Index].startDate);
        fprintf(fptr, "Event End Date: %s\n", list[Index].endDate);
        fprintf(fptr, "the event status: %s\n", statusEvent);
        
        if (list[Index].staffCount == 0)
        {
            fprintf(fptr, "No staff have been assigned to this event yet!\n");
        } else
        {   
            fprintf(fptr, "+---------------------------+--------------+------------+----------------------------------+\n");
            fprintf(fptr, "|        LIST OF STAFF FOR THE EVENT (Total: %d)                                         |\n", list[Index].staffCount);
            fprintf(fptr, "+---------------------------+--------------+------------+----------------------------------+\n");
            // Căn lề cố định: Tên (25 ký tự), ID (12 ký tự), Chức vụ (10 ký tự), Nhiệm vụ (30 ký tự)
            fprintf(fptr, "| %-25s | %-12s | %-10s | %-32s |\n", "Name", "ID", "Role", "Responsibilities");
            fprintf(fptr, "+---------------------------+--------------+------------+----------------------------------+\n");

            const char* ROLE_NAMES[] = {"BCN", "Member", "Support", "Unassigned"};

            for (int i = 0; i < list[Index].staffCount; i++)
            {   
                int r = list[Index].staffList[i].role;
                const char* roleStr = (r >= 0 && r <= 2) ? ROLE_NAMES[r] : ROLE_NAMES[3];

                const char* descStr = list[Index].staffList[i].description;
                if (descStr[0] == '\0') {
                    descStr = "No responsibilities assigned";
                }

                fprintf(fptr, "| %-25s | %-12s | %-10s | %-32s |\n", 
                        list[Index].staffList[i].studentName,
                        list[Index].staffList[i].studentId,
                        roleStr,
                        descStr);
            }
            fprintf(fptr, "+---------------------------+--------------+------------+----------------------------------+\n");
        }  
        fclose(fptr);
        
        printf("\033[1;32m>> Success:\033[0m Report saved successfully to '%s'\n", filename);
        
    } else
    {
        printf("\033[1;31m>> Error:\033[0m Event with the given ID or name does not exist!\n");
    }
}

void advancedStaffStatistics(Event events[], int eventCount, Account accounts[], int accountCount) {
    printf("\n============================================================\n");
    printf("                  ADVANCED STAFF STATISTICS                 \n");
    printf("============================================================\n");
    printf("\n--- 1. STAFF COUNT BY EVENT ---\n");
    if (eventCount == 0) {
        printf(">> Notice: No events in the system.\n");
    } else {
        printf("%-15s | %-30s | %-10s\n", "Event ID", "Event Name", "Staff Count");
        printf("------------------------------------------------------------\n");
        for (int i = 0; i < eventCount; i++) {
            printf("%-15s | %-30s | %d/30\n", events[i].eventId, events[i].name, events[i].staffCount);
        }
    }

    int participationCount[1000] = {0}; 
    int maxParticipation = 0;

    for (int i = 0; i < eventCount; i++) {
        for (int j = 0; j < events[i].staffCount; j++) {
            for (int k = 0; k < accountCount; k++) {
                if (strcmp(events[i].staffList[j].studentId, accounts[k].studentid) == 0) {
                    participationCount[k]++;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].role == 0 && participationCount[i] > maxParticipation) {
            maxParticipation = participationCount[i];
        }
    }
    printf("\n--- 2. MEMBERS WITH THE MOST PARTICIPATION ---\n");
    if (maxParticipation == 0) {
        printf(">> Notice: No members have participated in any events yet.\n");
    } else {
        printf(">> Maximum participation: %d event(s)\n", maxParticipation);
        for (int i = 0; i < accountCount; i++) {
            if (accounts[i].role == 0 && participationCount[i] == maxParticipation) {
                printf(" - Student ID: %-10s | Name: %s\n", accounts[i].studentid, accounts[i].username);
            }
        }
    }
    printf("\n--- 3. MEMBERS WITH NO PARTICIPATION ---\n");
    int lazyFound = 0;
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].role == 0 && participationCount[i] == 0) {
            printf(" - Student ID: %-10s | Name: %s\n", accounts[i].studentid, accounts[i].username);
            lazyFound = 1;
        }
    }
    if (!lazyFound) {
        printf(">> Awesome! All members have participated in at least one event!\n");
    }
    printf("============================================================\n");
}