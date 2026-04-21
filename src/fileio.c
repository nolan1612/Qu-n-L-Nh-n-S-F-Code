// Chuyên xử lý việc lưu mảng dữ liệu xuống file .dat và đọc lên
#include <stdio.h>
#include "fileio.h"

int saveAccount(Account arr[], int count){
    File *file = fopen("account.data", "w");
    if(file == NULL){
        printf("không thể mở account.data");
        return 0;
    }
    fwrite(arr, sizeof(Account), count, file);
    
   
    fclose(file);
    return 1;
}
int loadAccounts(Account arr[]) {
    FILE *file = fopen("account.dat", "rb");
    if (file == NULL) {
        return 0; 
    }
    int count = 0;
    while (fread(&arr[count], sizeof(Account), 1, file) == 1) {
        count++;
    }
    
    fclose(file);
    return count; 
}