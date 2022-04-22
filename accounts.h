#include <glib.h>
#include "shared.h"

#ifndef ACCOUNTS_H
#define ACCOUNTS_H

typedef struct {
    int ref;
    float value;
    char state[9];
    float debit_threshold;
} ACCOUNT;

GSList* get_accounts();

ACCOUNT* get_account(int);

int debit(int, float);

int credit(int, float);

#endif // ACCOUNTS_H
