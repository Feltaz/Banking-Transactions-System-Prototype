#include <glib.h>

#ifndef ACCOUNTS_H
#define ACCOUNTS_H

typedef struct {
    int ref;
    int value;
    char state[9];
    int debit_threshold;
} ACCOUNT;

GSList* get_accounts();

ACCOUNT* get_account(int ref);

int debit(int ref, int value);

int credit(int ref, int value);

#endif // ACCOUNTS_H
