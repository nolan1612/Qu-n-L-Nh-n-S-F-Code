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
#include "../includes/request.h"

Account list[MAX_ACCOUNTS];
Event eventList[MAX_EVENTS];
Account *currentAcc;
Request listRequests[MAX_REQUESTS];
int main() {
    char mssv[20];
    char ps[20];
    int choice;
    
    int accountCount = loadAccounts(list);
    int eventCount = loadEvents(eventList);
    int requestCount = loadRequests(listRequests);
    printf("[System] Successfully loaded %d accounts, %d events, and %d requests from file.\n", accountCount, eventCount, requestCount);

    while (1) {
        printf("\n=========================================\n");
        printf("         WELCOME TO F-CODE STAFF         \n");
        printf("=========================================\n");
        printf("1. Login\n");
        printf("2. Forgot Password\n");
        printf("3. Register\n");
        printf("0. Exit program\n");
        printf("-----------------------------------------\n");
        printf("Your choice: ");
        
        choice = validInput(0,3);

        if (choice == 0) {
            printf("Goodbye! See you again.\n");
            break;
        } else if (choice == 1) {
                printf("\n--- LOGIN ---\n");

               while (1) {

                        printf("Nhap MSSV: ");
                        scanf("%s", mssv);

                        int index = checkId(mssv, list, accountCount);

                        if (index != -1) {
                            break;
                        }
                        printf(">> Error: Student ID does not exist! Please try again.\n");
                    }   

                while(1) 
                {  
                    printf("Enter password: ");
                    scanf(" %19[^\n]", ps);
                    
                    int status = Login(mssv, ps, list, accountCount);
                
                    if (status >= 0) {
                        printf("\n>>> Login successful! <<<\n");
                        if (list[status].role == 1) {
                            runAdminMenu(&list[status], list, accountCount, eventList, &eventCount);
                        } else {
                            runMemberMenu(&list[status], list, accountCount, eventList, eventCount);
                        }
                        saveAccounts(list, accountCount); 
                        break;
                    } else if (status == -1) {
                        saveAccounts(list, accountCount);
                    } else if (status == -3) {                                   
                        saveAccounts(list, accountCount);
                        int lockedIndex = checkId(mssv, list, accountCount);
                        runRequest(&list[lockedIndex], listRequests, accountCount);

                    	if (choice == -1) {
                            printf(">> Returning to welcome menu...\n");
                        }
                                            
                    } 
                }
            } else if (choice == 2) {
                forgotPassword(list, accountCount);
            } else if (choice == 3) {
                Register(list, &accountCount);
                saveAccounts(list, accountCount);
            } 
    }
    return 0;
}