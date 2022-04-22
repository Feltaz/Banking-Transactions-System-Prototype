#include <glib.h>

#ifndef BILLS_H
#define BILLS_H

typedef struct {
    int ref;
    float debt;
} BILL;

GSList* get_bills();

int add_bill(int ref, float debt);

BILL* get_bill(int ref);

#endif // BILLS_H
