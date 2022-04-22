#include <glib.h>
#include "shared.h"

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

typedef struct {
    int ref;
    float value;
    char transaction[8];
    char result[8];
    char state[9];
} TRANSACTION;

GSList* get_transactions();

int add_transaction(int, const char*, float, const char*, const char*);

#endif // TRANSACTIONS_H
