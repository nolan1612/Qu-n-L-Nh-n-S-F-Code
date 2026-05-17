#include "../includes/auth.h"
#include "../includes/event.h"
#include "../includes/menu.h"
#include "../includes/fileio.h"
#include "../includes/report.h"
#include "../includes/staff.h"
#include "../includes/utils.h"

void saveAccounts(Account list[], int count) {
    FILE *file = fopen("data/account.dat", "w"); 
    if (file == NULL) {
        printf("\033[1;31m[Error]\033[0m Cannot open file data/account.dat for writing!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%s|%s|%s|%d|%d|%d\n", 
                list[i].studentid, 
                list[i].username,
                list[i].password, 
                list[i].email,
                list[i].phone,
                list[i].role, 
                list[i].isLocked, 
                list[i].failCount);
    }
    fclose(file);
}
int loadAccounts(Account list[]) {
    FILE *file = fopen("data/account.dat", "r"); 
    
    if (file == NULL) {
        printf("[Error] File data/account.dat not exist!\n");
        return 0; 
    }
    
    int count = 0;
    while (fscanf(file, " %19[^|]|%49[^|]|%19[^|]|%49[^|]|%19[^|]|%d|%d|%d\n", 
                  list[count].studentid, 
                  list[count].username,   
                  list[count].password, 
                  list[count].email,
                  list[count].phone,
                  &list[count].role, 
                  &list[count].isLocked, 
                  &list[count].failCount) == 8) { 
        count++;
        if (count >= MAX_ACCOUNTS) break; 
    }
    
    fclose(file);
    return count;
}

void saveEvents(Event list[], int count) {
    FILE *file = fopen("data/events.dat", "wb"); 
    if (file == NULL) {
        printf("\033[1;31m[Error]\033[0m Can not open file events.dat for writing!\n");
        return;
    }
    size_t written = fwrite(list, sizeof(Event), count, file);
    if (written != (size_t)count) {
        printf("\033[1;31m[Error]\033[0m Failed to write all events to file!\n");
    }
    fclose(file);
}

int loadEvents(Event list[]) {
    FILE *file = fopen("data/events.dat", "rb"); 
    if (file == NULL) {
        return 0; 
    }
    int count = 0;
    while (fread(&list[count], sizeof(Event), 1, file) == 1) {
        count++;
        if (count >= MAX_EVENTS) break;
    }
    fclose(file);
    return count;
}
