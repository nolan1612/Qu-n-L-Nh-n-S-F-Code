//  Định nghĩa struct StaffEntry (MSSV, Công việc, Đánh giá...)
#ifndef STAFF_H
#define STAFF_H

#include "event.h"

void toUpperCaseStr(char str[]);
void editStaffRole(Event events[], int count);
void removeStaffFromEvent(Event events[], int count);
void viewMemberHistory(Event events[], int count);
void addStaffToEvent(Event events[], int count, Account list[], int accountCount);
void eventJoin(Event events[], int eventCount, Account *currentAcc);
void approveJoinRequests(Event events[], int count);
#endif