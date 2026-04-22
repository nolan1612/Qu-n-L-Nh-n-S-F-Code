#include <stdio.h>
#include "fileio.h"
#include "auth.h"

void saveAccounts(Account list[], int count) {

    FILE *file = fopen("accounts.dat", "wb"); 
    if (file == NULL) {
        printf("Loi: Khong the ghi file accounts.dat!\n");
        return;
    }
    fwrite(list, sizeof(Account), count, file);
    fclose(file);
}

int loadAccounts(Account list[]) {
    FILE *file = fopen("accounts.dat", "rb"); 
    if (file == NULL) {
        printf("[He thong] File accounts.dat chua ton tai. Dang tao du lieu mau...\n");
        
        Account mockData[5] = {
            {"SE211377", "Ho26072007*", 1, 0, 0}, 
            {"SE210023", "1234567789", 0, 0, 0},  
            {"SE211577", "234567890", 0, 0, 0},
            {"SE210182", "345678901", 0, 0, 0},
            {"SE205432", "ui1234567", 1, 0, 0}
        };
        
        for (int i = 0; i < 5; i++) {
            list[i] = mockData[i];
        }
        
        saveAccounts(list, 5);
        return 5; 
    }
    
    int count = 0;
    while (fread(&list[count], sizeof(Account), 1, file) == 1) {
        count++;
    }
    fclose(file);
    return count;
}