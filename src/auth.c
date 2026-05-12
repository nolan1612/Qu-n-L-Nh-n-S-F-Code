#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../includes/auth.h"
#include "../includes/event.h"
#include "../includes/menu.h"
#include "../includes/fileio.h"
#include "../includes/report.h"
#include "../includes/staff.h"
#include "../includes/utils.h"
int checkPassword(char ps[], Account *account);
int Login(char mssv[], char ps[], Account list[], int accountCount) ;
void changePassword(Account *currentAcc);
int Logout(Account *currentAcc, Account list[], int accountCount);
void forgotPassword(Account list[], int accountCount);
void setupEmail(Account *account, Account list[], int accountCount);
void generatedSecretKey(int lenght);
void Register(Account list[], int *accountCount);
int checkPassword(char ps[], Account *account) {
    if (strcmp(ps, account->password) == 0) {
        account->failCount = 0; 
        return 1; 
    }

    account->failCount++;
    if (account->failCount >= 3) {
        account->isLocked = 1;
        printf("Incorrect password 3 times. This account has been locked!\n");
        return -3;
    } 
    printf("Incorrect password! You have %d attempts left.\n", 3 - account->failCount);
      return -1;
}

int Login(char mssv[], char ps[], Account list[], int accountCount) {    
    int index = -1;
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(mssv, list[i].studentid) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Student ID does not exist!\n");
        return -2;
    }
    if (list[index].isLocked == 1) {
        printf("This account has been locked!\n");
        return -3; 
    }

    int result = checkPassword(ps, &list[index]);
    if (result == 1) return index;
    if (result == -1) return -1;   
    if (result == -3) return -3;
    return -1; 
}

    void changePassword(Account *currentAcc) {
        char oldPass[20], newPass[20], confirmPass[20];
        int failCount = 0; 
        
        printf("\n--- CHANGE PASSWORD ---\n");

    while(1) {
        printf("Enter old password (Type '0' to cancel): ");
        scanf(" %s", oldPass);

        if (strcmp(oldPass, "0") == 0) {
            printf(">> Change password canceled. Returning to menu...\n");
            return; 
        }

        if (strcmp(oldPass, currentAcc->password) != 0) {
            failCount++; 
            printf(">> Error: Incorrect old password!\n");
            
            
            if (failCount >= 3) {
                printf("\033[1;33m[Tip]\033[0m If you forgot your password, type '0' to exit\n");
            } else {
                printf(">> Please try again.\n");
            }
        } else {
            break; 
        }
    }

        while(1) {
            printf("Enter new password (Type '0' to cancel): "); 
            scanf(" %[^\n]", newPass);
            
            if (strcmp(newPass, "0") == 0) {
                printf(">> Change password canceled. Returning to menu...\n");
                return; 
            }

            if (strcmp(newPass, currentAcc->password) == 0) {
                printf(">> Error: New password cannot be the same as the current password!\n");
                continue; 
            }

            printf("Confirm new password: ");
            scanf(" %[^\n]", confirmPass);

            if (strcmp(confirmPass, "0") == 0) {
                printf(">> Change password canceled. Returning to menu...\n");
                return; 
            }

            if (strcmp(newPass, confirmPass) != 0) {
                printf(">> Error: New password and confirmation do not match!\n");
            } 
            else{
                strcpy(currentAcc->password, newPass);
                printf("\033[1;32m>> Success: Password has been changed!\033[0m\n");
                break; 
            }
        }
    }

int Logout(Account *currentAcc, Account list[], int accountCount) {
    printf("Confirm logout? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        saveAccounts(list, accountCount);
        printf(">> Logged out successfully. See you again %s!\n", currentAcc->studentid);
        return 1;
    } else {
        printf(">> Logout canceled.\n");
        return 0;
    }
}
void forgotPassword(Account list[], int accountCount){
    char inputEmail[101];
    char foundIndex = -1;
    printf("==FORGOT PASSWORD==\n");
    printf("plz,input your email\n");
    scanf(" %[^\n]", inputEmail);
    
    if (strcmp(inputEmail, "0") == 0) {
        printf(">> Action canceled. Returning to menu...\n");
        return;
    }
    if (isValidEmail(inputEmail) == 0) {
        printf(">> Error: Invalid email format!\n");
        return;
    }
    for(int i = 0; i <= accountCount - 1; i++){
        if(strcmp(list[i].email, inputEmail) == 0){
            foundIndex = i;
            break;
        }
    }
    if (foundIndex == -1) {
        printf(">> Error: Email does not exist in the system!\n");
        return;
    }
    char newPass[20], confirmPass[20];
    while(1) {
        printf(">> Account found! Enter new password (Type '0' to cancel): ");
        scanf(" %[^\n]", newPass); 
        
        if (strcmp(newPass, "0") == 0) {
            printf(">> Action canceled. Returning to menu...\n");
            return;
        }

        printf("Confirm new password: ");
        scanf(" %[^\n]", confirmPass);

        if (strcmp(newPass, confirmPass) != 0) {
            printf(">> Error: Password confirmation does not match. Please try again!\n");
        } else {
            strcpy(list[foundIndex].password, newPass);
            list[foundIndex].failCount = 0;
            list[foundIndex].isLocked = 0;
            saveAccounts(list, accountCount);
            printf("\033[1;32m>> Success: Your password has been successfully recovered!\033[0m\n");
            break; 
        }
    }
}
void setupEmail(Account *account, Account list[], int accountCount) {
    printf("=== Setup Email ===\n");

    char email[100];

    while (1) {
        printf("Input new email: ");
        scanf(" %99[^\n]", email);

        if (isValidEmail(email)) {
            break; // hợp lệ → thoát vòng lặp
        }

        printf("Invalid email! Please try again.\n");
    }

    strncpy(account->email, email, sizeof(account->email) - 1);
    account->email[sizeof(account->email) - 1] = '\0';

    saveAccounts(list, accountCount);
    printf("Email setup successfully!\n");
}
void generatedSecretKey(int lenght){
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char newKey[50];

    srand(time(NULL));
    for(int i = 0; i <= lenght - 1; i++){
        newKey[i] = charset[rand() % (sizeof(charset) - 1)];
    }
     newKey[lenght] = '\0';
     FILE *f = fopen("secret.dat", "w");
     if(f != NULL){
        fprintf(f, "%s", newKey);
        fclose(f);
        printf("Successfully generated secret key and your key is: %s\n", newKey);
     }else{
        printf("Error: Unable to generate secret key!\n");
     }
}
void Register(Account list[], int *accountCount){
    if(*accountCount >= MAX_ACCOUNTS){
        printf(">> Error: Maximum number of accounts reached! Cannot register new account.\n");
        return;
    }
    char inputidStuden[20];
    char inputUsername[50];
    char inputEmail[50];
   // char inputPhoneNumber[20];
    char inputSecretKey[50];
    printf("=== REGISTER NEW ACCOUNT ===\n");
    while(1){
        printf("Input student ID:");
        scanf(" %19[^\n]", inputidStuden);
       clearBuffer();
       if(strcmp(inputidStuden, "0") == 0){
        printf(">> Action canceled. Returning to menu...\n");
        return;
       }
       int exist = 0;
       for(int i = 0; i <= *accountCount - 1; i++){
        if(strcmp(list[i].studentid, inputidStuden) == 0){
            exist = 1;
            break;
        }
       }
       if(exist){
            printf(">> Error: Student ID already exists! Please try again.\n");
            continue;
       }
       break;
    }
    
        printf("Input username:");
        scanf(" %49[^\n]", inputUsername);
        clearBuffer();
        printf("Input password:");
        char inputPassword[20];
        scanf(" %19[^\n]", inputPassword);
        clearBuffer();
        
    while(1){
        printf("Input email:");
        scanf(" %49[^\n]", inputEmail);
        clearBuffer();
        if (strcmp(inputEmail, "0") == 0) {
            return; 
        }
        if (isValidEmail(inputEmail) == 0) {
            printf(">> Error: Invalid email format! Please try again.\n");
            continue;
        }
        int isDuplicate = 0; 
        for (int i = 0; i < *accountCount; i++) {
            if (strcmp(list[i].email, inputEmail) == 0) {
                isDuplicate = 1; 
                break;
            }
        }
        if (isDuplicate == 1) {
            printf(">> Error: Email already exists in the system!\n");
            continue;
        }
        printf(">> Successfully input email!\n");
        break;
    }
    // while(1){
    //     printf("Input phone number:");
    //     scanf(" %19[^\n]", inputPhoneNumber);
    //     clearBuffer();
    //     if (strcmp(inputPhoneNumber, "0") == 0) {
    //         return; 
    //     }
    //     if(isValidphoneNumber(inputPhoneNumber) == 0){
    //         printf(">> Error: Invalid phone number format! Please try again.\n");
    //         continue;
    //     }
    //     int duplicate = 0;
    //     for(int i = 0; i <= *accountCount - 1; i++){
    //         if(strcmp(list[i].phone, inputPhoneNumber) == 0){
    //             duplicate = 1;
    //             break;
    //         }
    //     }
    //     if (duplicate == 1) {
    //         printf(">> Error: Phone number already exists in the system!\n");
    //         continue;
    //     }
    //     printf(">> Successfully input phone number!\n");
    //     break;
    // }
    int finalrole = 0;
    printf("\n>> [Security] Do you have the admin secret key?");
    printf("\n>> If you are a member, just press '0' to skip this step and your account will be created with MEMBER role.\n");
    scanf(" %49[^\n]", inputSecretKey);
    clearBuffer();
    if (strcmp(inputSecretKey, "0") == 0){
        printf(">> Your account was successfully created with MEMBER role!\n");
    } else{
        char realKey[50];
        FILE *f = fopen("secret.dat", "r");
        if(f != NULL){
            fscanf(f, "%49s", realKey);
            fclose(f);
        }
        if(strlen(realKey) > 0 && strcmp(inputSecretKey, realKey) == 0){
            finalrole = 1;
            printf(">> Your account was successfully created with ADMIN role!\n");
            f = fopen("secret.data", "w");
            if (f != NULL) {
                fprintf(f, "USED");
                fclose(f);
        } else{
            printf(">> Warning: Secret key is uncorrect or has already been used!\n");
            printf(">> Error: Incorrect secret key! Your account will be created with MEMBER role.\n");
        }
    }
    strcpy(list[*accountCount].studentid, inputidStuden);
    strcpy(list[*accountCount].username, inputUsername);
    strcpy(list[*accountCount].email, inputEmail);
    //strcpy(list[*accountCount].phone, inputPhoneNumber);
    list[*accountCount].role = finalrole;
    list[*accountCount].failCount = 0;
    list[*accountCount].isLocked = 0;
    (*accountCount)++;
    saveAccounts(list, *accountCount);
    printf("\n\033[1;32m>>Success: Account created successfully!\033[0m\n");
}
}