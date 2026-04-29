// Khai báo các hàm dùng chung
#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
void clearBuffer();
void trimNewLine(char str[]);
int validInput(int min, int max);
#endif