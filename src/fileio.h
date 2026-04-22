#ifndef FILEIO_H
#define FILEIO_H

#include "auth.h"

void saveAccounts(Account list[], int count);
int loadAccounts(Account list[]);

void saveEvents(Event list[], int count);
int loadEvents(Event list[]);

#endif