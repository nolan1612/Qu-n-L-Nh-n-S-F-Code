//  Định nghĩa struct Account (MSSV, Mật khẩu, Quyền)
#ifndef AUTH_H
#define AUTH_H

#define MAX_ACCOUNTS 500
#define MAX_FAIL_ATTEMP 3

typedef struct {
    char studentid[20];
    char username[50];
    char password[20];
    char email[50];
    char phone[20];
    char dept[50];
    int role; 
    int isLocked; 
    int failCount;
} Account;

int checkPassword(char ps[], Account *account);
int Login(char mssv[], char ps[], Account list[], int accountCount);
void changePassword(Account *currentAcc);
int Logout(Account *currentAcc, Account list[], int accountCount);
void forgotPassword(Account list[], int accountCount);
void setupEmail(Account *account, Account list[], int accountCount);
void generatedSecretKey(int lenght);
void Register(Account list[], int *accountCount);
int checkPassword(char ps[], Account *account);
#endif
