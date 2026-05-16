// Khai báo các hàm dùng chung
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "auth.h"
#include "validate.h"



void clearBuffer();
void trimNewLine(char str[]);
int validInput(int min, int max);
int containsIgnoreCase(char str[], char strSub[]);
int confirmAction( char message[]);
int isLeapYear(int year);
int isValidDateNum(int year, int month, int day);
int isValidDateStr(const char* date);
int getDaysDifference(const char* start, const char* end);
void inputValidFormatDate(char str[]);
int checkPassword(char ps[], Account *account);
int getSearchScore(const char eventName[], const char searchInput[]);
void toLowerCase(char str[]);
int checkPassword(char ps[], Account *account);
int isValidEmail( char *email);
void inputString(char str[], int size);
int checkId(char mssv[], Account list[], int accountCount);
#endif
