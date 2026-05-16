// Khai bao cac tinh nang nang cao
#ifdef _WIN32
    #include <direct.h> 
#else
    #include <sys/stat.h> 
#endif
#ifndef REPORT_H
#define REPORT_H

#include "event.h" 

int eventSearch(Event list[], int countEvent, char eIdorName[]);
void eventDetail(Event list[], int countEvent);
void createFile(Event list[], int countEvent);
void advancedStaffStatistics(Event events[], int eventCount, Account accounts[], int accountCount);
#endif