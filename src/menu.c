#include <stdio.h>
#include "../includes/auth.h"
#include "../includes/event.h"
#include "../includes/menu.h"
#include "../includes/fileio.h"
#include "../includes/report.h"
#include "../includes/staff.h"
#include "../includes/utils.h"
#include "../includes/request.h"

void AdminMenu() {
    printf("\n=========================================\n");
    printf("            ADMIN MENU                   \n");
    printf("=========================================\n");
    printf("1. Create new event\n");
    printf("2. Edit event information\n");
    printf("3. Delete event\n");
    printf("4. Update event status\n");
    printf("5. Manage staff (Add/Edit/Delete)\n");
    printf("6. View all events list\n");
    printf("7. View event details\n");
    printf("8. Search events\n");
    printf("9. View member participation history\n");
    printf("10. Change password\n");
    printf("11. Setup/Update Email\n"); 
    printf("12. Export event report\n");
    printf("13. Search events by time range\n");
    printf("14. View unlock requests\n");
    printf("15. Generate admin secret Key\n");
    printf("16. Approve join requests\n");
    printf("17. Advanced staff statistics\n");
    printf("0. Logout\n");
    printf("=========================================\n");
}

void MemberMenu() {
    printf("\n=========================================\n");
    printf("             MEMBER MENU                 \n");
    printf("=========================================\n");
    printf("1. View personal profile\n");
    printf("2. View joined events list\n");
    printf("3. View joined event details\n");
    printf("4. View participation history\n");
    printf("5. Change password\n");
    printf("6. Setup/Update Email\n"); 
    printf("7. Submit unlock request\n");
    printf("8. Join an event\n");
    printf("0. Logout\n");
    printf("=========================================\n");
}

void runAdminMenu(Account *currentAcc, Account list[], int accountCount, Event eventList[], int *eventCount) {
    int choice;
    do {
        AdminMenu();
        printf("Enter your choice (0-17): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }

        switch(choice) {
            case 1: 
                createEvent(eventList, eventCount);
                break;
            case 2: 
                editEvent(eventList, *eventCount);
                break;
            case 3: 
                deleteEvent(eventList, eventCount);
                break;
            case 4: 
                updateEventStatus(eventList, *eventCount);
                break;
            case 5: {
                int subChoice;
                do {
                    printf("\n--- STAFF MANAGEMENT ---\n");
                    printf("1. Add staff to event\n");
                    printf("2. Edit staff role in event\n");
                    printf("3. Remove staff from event\n");
                    printf("0. Back to main menu\n");
                    printf("Enter your choice: ");
                    if (scanf("%d", &subChoice) != 1) {
                        while(getchar() != '\n');
                        subChoice = -1;
                    }

                    switch(subChoice) {
                        case 1: 
                            addStaffToEvent(eventList, *eventCount, list, accountCount); 
                            break;
                        case 2: 
                            editStaffRole(eventList, *eventCount); 
                            break;
                        case 3: 
                            removeStaffFromEvent(eventList, *eventCount); 
                            break;
                        case 0: 
                            printf(">> Returning to Admin menu...\n");
                            break;
                        default: 
                            printf(">> Error: Invalid choice. Please try again!\n");
                    }
                } while (subChoice != 0);
                break;
            }
            case 6: 
                displayAllEvents(eventList, *eventCount);
                break;
            case 7: 
                eventDetail(eventList, *eventCount);
                break;
            case 8: 
                searchEventsByNameOrId(eventList, *eventCount);
                break;
            case 9: 
                viewMemberHistory(eventList, *eventCount);
                break;
            case 10: 
                changePassword(currentAcc);
                saveAccounts(list, accountCount);
                break;
            case 11: 
                setupEmail(currentAcc, list, accountCount);
                break;
            case 15:
                generatedSecretKey(6);
                break;
            case 0: 
                if (Logout(currentAcc, list, accountCount) == 1) {
                    return; 
                }
                choice = -1; 
                break;
            case 12:
            	createFile(eventList, *eventCount);
            	break;
            case 13:
            	searchEventsByTimeRange(eventList, *eventCount);
            	break;
            case 14:
            	viewUnlockRequests(list, accountCount);
                saveAccounts(list, accountCount);
            	break;
            case 16:
                approveJoinRequests(eventList, *eventCount, list, accountCount);
                break;
            case 17:
                advancedStaffStatistics(eventList, *eventCount, list, accountCount);
                break;
            default: 
                printf(">> Error: Invalid choice. Please try again!\n");
        }
    } while (choice != 0); 
}

void runMemberMenu(Account *currentAcc, Account list[], int accountCount, Event eventList[], int eventCount) {
    int choice;
    do {
        MemberMenu();
        printf("Enter your choice (0-8): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }

        switch(choice) {
            case 1:
                viewMemberProfile(currentAcc);
                break;
            case 2:
                viewJoinedEventsList(eventList, eventCount, currentAcc);
                break;
            case 3:
                viewJoinedEventDetails(eventList, eventCount, currentAcc);
                break;
            case 4:
                viewMyParticipationHistory(eventList, eventCount, currentAcc);
                break;
            case 5:
                changePassword(currentAcc);
                saveAccounts(list, accountCount);
                break;
            case 6:
                setupEmail(currentAcc, list, accountCount);
                break;
            case 7:
                if (currentAcc->isLocked != 0) {
                    printf(">> Your account is not locked!\n");
                } else {
                    printf(">> Please logout and login again to submit an unlock request.\n");
                }
                break;
            case 8:
                eventJoin(eventList, eventCount, currentAcc);
                break;
            case 0:
                if (Logout(currentAcc, list, accountCount) == 1) {

                    return; 
                }
                choice = -1;
                break;
            default:
                printf(">> Error: Invalid choice. Please try again!\n");
        }
    } while (choice != 0);
}


void runRequest(Account *currentAcc, Request listRequests[], int accountCount)
{
    int choice;
    do 
    {
        printf("Your account is lock\n");
        printf("1. Send request to open your account\n");
        printf("0. Back to wecome menu\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) 
        {
            while (getchar() != '\n');
            choice = -1;
        }

        switch (choice) 
        {
        case 1:
            sendUnlockRequest(currentAcc);
            saveRequests(listRequests, accountCount);
            break;
        case 0:
            choice = -1;
            break;
        default:
            printf(">> Error: Invalid choice. Please try again!\n");
        }
    } while (choice != 0);
}
