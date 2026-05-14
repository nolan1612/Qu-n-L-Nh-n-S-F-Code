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
int Login(char mssv[], char ps[], Account list[], int accountCount) ;
void changePassword(Account *currentAcc);
int Logout(Account *currentAcc, Account list[], int accountCount);
void forgotPassword(Account list[], int accountCount);
void setupEmail(Account *account, Account list[], int accountCount);
void generatedSecretKey(int lenght);
void Register(Account list[], int *accountCount);

int Login(char mssv[], char ps[], Account list[], int accountCount) {    
    int index = -1;
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(mssv, list[i].studentid) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return -2;
    }
    if (list[index].isLocked == 1) {
        printf("This account has been locked!\n");
        printf("Do you want to send an unlock request to the Admin?\n");
        if (confirmAction("Send request?")) {
            list[index].isLocked = 2; 
            saveAccounts(list, accountCount);
            printf(">> Request sent. Please wait for the Admin to approve!\n");
        }
        return -3; 
    }

    if (list[index].isLocked == 2) {
        printf("Account is waiting for Admin to approve the unlock request!\n");
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
        printf("Enter old password: ");
        inputString(oldPass, sizeof(oldPass));

        if (strcmp(oldPass, "0") == 0) {
            printf(">> Change password canceled. Returning to menu...\n");
            return; 
        }

        if (strcmp(oldPass, currentAcc->password) != 0) {
            failCount++; 
            printf(">> Error: Incorrect old password!\n");
            printf(">> Please try again. You have %d attempts left.\n", 3 - failCount);
            
           
            if (failCount >= 3) {
                printf("\033[1;33m[Tip]\033[0m If you forgot your password, type '0' to exit and ask Admin to reset it!\n");
            }
        } else {
            break; 
        }
    }


    while(1) {
        printf("Enter new password (Type '0' to cancel): "); 
        inputString(newPass, sizeof(newPass));

        if (strcmp(newPass, "0") == 0) {
            printf(">> Change password canceled. Returning to menu...\n");
            return; 
        }

        if (strcmp(newPass, currentAcc->password) == 0) {
            printf(">> Error: New password cannot be the same as the current password!\n");
            continue; 
        }

        printf("Confirm new password: ");
        inputString(confirmPass, sizeof(confirmPass));

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
    char choiceInput[10];
    inputString(choiceInput, sizeof(choiceInput));
    char choice = choiceInput[0];
    if (choice == 'y' || choice == 'Y') {
        saveAccounts(list, accountCount);
        printf(">> Logged out successfully. See you again, %s!\n", currentAcc->username);
        return 1;
    } else {
        printf(">> Logout canceled.\n");
        return 0;
    }
}

void forgotPassword(Account list[], int accountCount){
    char inputEmail[101];
    int foundIndex = -1;
    printf("==FORGOT PASSWORD==\n");
    printf("plz,input your email\n");
    inputString(inputEmail, sizeof(inputEmail));
    
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
        inputString(newPass, sizeof(newPass));

        if (strcmp(newPass, "0") == 0) {
            printf(">> Action canceled. Returning to menu...\n");
            return;
        }

        printf("Confirm new password: ");
        inputString(confirmPass, sizeof(confirmPass));
        confirmPass[strcspn(confirmPass, "\n")] = '\0';

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
        inputString(email, sizeof(email));

        EmailValidationResult emailStatus = validateEmail(email);
        if (emailStatus != EMAIL_VALID) {
            printf("%s Please try again.\n", getEmailErrorMessage(emailStatus));
            continue;
        }
        break;
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
    char inputPassword[20];
    char inputEmail[50];
    char inputPhoneNumber[20];
    char inputSecretKey[50];
    
    printf("=== REGISTER NEW ACCOUNT ===\n");
    while(1){
        printf("Input student ID:");
        inputString(inputidStuden, sizeof(inputidStuden));
       if(strcmp(inputidStuden, "0") == 0){
        printf(">> Action canceled. Returning to menu...\n");
        return;
       }
       StudentIdValidationResult idStatus = validateStudentID(inputidStuden);
       if(idStatus != STUDENT_ID_VALID){
            printf("%s Please try again.\n", getStudentIDErrorMessage(idStatus));
            continue;
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
    
    while(1){
        printf("Input username:");
        inputString(inputUsername, sizeof(inputUsername));
        if(strcmp(inputUsername, "0") == 0){
        printf(">> Action canceled. Returning to menu...\n");
        return;
         }
       
         NameValidationResult nameStatus = validateName(inputUsername);
         if(nameStatus != NAME_VALID){
            printf("%s Please try again.\n", getNameErrorMessage(nameStatus));
            continue;
         }
         break;
    }
    
    while(1){
        printf("Input password:");
        inputString(inputPassword, sizeof(inputPassword));
        if(strcmp(inputPassword, "0") == 0){
        printf(">> Action canceled. Returning to menu...\n");
        return;
        }
        PasswordValidationResult passStatus = validatePassword(inputPassword);
        if(passStatus != PASSWORD_VALID){
            printf("%s Please try again.\n", getPasswordErrorMessage(passStatus));
            continue;
        }
        break;
    }

    while(1){
        printf("Input email:");
        inputString(inputEmail, sizeof(inputEmail));
        if (strcmp(inputEmail, "0") == 0) {
            return; 
        }
        EmailValidationResult emailStatus = validateEmail(inputEmail);
        if (emailStatus != EMAIL_VALID) {
            printf("%s Please try again.\n", getEmailErrorMessage(emailStatus));
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
       
        break;
    }

    while(1){
        printf("Input phone number:");
        inputString(inputPhoneNumber, sizeof(inputPhoneNumber));
        if (strcmp(inputPhoneNumber, "0") == 0) {
            return; 
        }
        PhoneValidationResult phoneStatus = validatePhone(inputPhoneNumber);
        if (phoneStatus != PHONE_VALID) {
            printf("%s Please try again.\n", getPhoneErrorMessage(phoneStatus));
            continue;
        }
        int duplicate = 0;
        for(int i = 0; i <= *accountCount - 1; i++){
            if(strcmp(list[i].phone, inputPhoneNumber) == 0){
                duplicate = 1;
                break;
            }
        }
        if (duplicate == 1) {
            printf(">> Error: Phone number already exists in the system!\n");
            continue;
        }
    
        break;
    }
    int finalrole = 0;
    printf("\n>> [Security] Do you have the admin secret key?");
    printf("\n>> If you are a member, just press '0' to skip this step and your account will be created with MEMBER role.\n");
    inputString(inputSecretKey, sizeof(inputSecretKey));
    if (strcmp(inputSecretKey, "0") == 0){
        printf(">> Your account was successfully created with MEMBER role!\n");
    } else{
        char realKey[50] = "";
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
            }
        } else{
            printf(">> Warning: Secret key is uncorrect or has already been used!\n");
            printf(">> Error: Incorrect secret key! Your account will be created with MEMBER role.\n");
        }
    }
    strcpy(list[*accountCount].studentid, inputidStuden);
    strcpy(list[*accountCount].username, inputUsername);
    strcpy(list[*accountCount].password, inputPassword);
    strcpy(list[*accountCount].email, inputEmail);
    strcpy(list[*accountCount].phone, inputPhoneNumber);
    list[*accountCount].role = finalrole;
    list[*accountCount].failCount = 0;
    list[*accountCount].isLocked = 0;
    (*accountCount)++;
    saveAccounts(list, *accountCount);
    printf("\n\033[1;32m>>Success: Account created successfully!\033[0m\n");
}
