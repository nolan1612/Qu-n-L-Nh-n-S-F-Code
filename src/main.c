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

int main() {
    Account list[MAX_ACCOUNTS];
    Event eventList[MAX_EVENTS];
    char mssv[10];
    char ps[20];
    int choice;
    
    int accountCount = loadAccounts(list);
    int eventCount = loadEvents(eventList);

    printf("[He thong] Da tai thanh cong %d tai khoan tu file.\n", accountCount);
    printf("[He thong] Da tai thanh cong %d su kien tu file.\n", eventCount);

	while (1) {
        printf("\n=========================================\n");
        printf("   WELCOME TO F-CODE STAFF   \n");
        printf("=========================================\n");
        printf("1. Log in\n");
        printf("0. Thoat chuong trinh\n");
        printf("-----------------------------------------\n");
        printf("Lua chon cua ban: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (choice == 0) {
            printf("Tam biet! Hen gap lai.\n");
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
                    } else if (status == -1) 
                            {
                                saveAccounts(list, accountCount);
                            } else if (status == -3) 
                                        {                                   
                                            saveAccounts(list, accountCount);
                                            runRequest(currentAcc, listRequests, accountCount);

                                            if (choice == -1) 
                                            {
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

            int status ;
            do{
                printf("Nhap mat khau: ");
                scanf(" %[^\n]", ps);
                status = Login(mssv, ps, list, accountCount);
                if(status == -1) {
                    saveAccounts(list, accountCount);
                    printf(">> Dang nhap that bai! Vui long thu lai.\n");
                } else if (status == -3) {
                    saveAccounts(list, accountCount);
                }
            } while (status == -1); // nếu sai pass thì cho nhập lại, nếu có lỗi khác thì thoát

            if (status >= 0) {
                printf("\n>>> Dang nhap thanh cong! <<<\n");
                if (list[status].role == 1) {
                    runAdminMenu(&list[status], list, accountCount, eventList, &eventCount);
                } else {
                    runMemberMenu(&list[status], list, accountCount);
                }
                saveAccounts(list, accountCount); 
            } else if (status == -2) {
                printf("MSSV khong ton tai!\n");
            }else if(status == -3) {
                printf("Tai khoan bi khoa!\n");
            }

        } else{
            printf(">> Loi: Lua chon khong hop le!\n");
        }
    }
    return 0;
}