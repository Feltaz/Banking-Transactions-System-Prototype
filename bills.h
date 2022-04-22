#include <glib.h>

#ifndef BILLS_H
#define BILLS_H

typedef struct {
    int ref;
    float debt;
} BILL;

GSList* get_bills();

int increment_bill(int, float);

BILL* get_bill(int);

#endif // BILLS_H
