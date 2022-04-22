#include <glib.h>
#include "shared.h"

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

typedef struct {
    int ref;
    int value;
    char transaction[8];
    char result[8];
    char state[9];
} TRANSACTION;

GSList* get_transactions();

int add_transaction(int ref, char* transaction, int value, char* result, char* state);

#endif // TRANSACTIONS_H
