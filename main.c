#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "accounts.h"

typedef struct {
    int ref;
    float debt;
} BILLING;

void print_account(ACCOUNT* account){
    printf("Account Details: \n");
    printf("Reference: %d\n", account->ref);
    printf("Value: %d\n", account->value);
    printf("State: %s\n", account->state);
    printf("Debit Threshold: %d\n", account->debit_threshold);
    printf("\n");
}

int main()
{
    /*ACCOUNT* account1 = (ACCOUNT*)malloc(sizeof(ACCOUNT));
    account1->ref = 1234;
    account1->value = 300;
    strcpy(account1->state, STATE_POSITIVE);
    account1->debit_threshold = 0;

    ACCOUNT* account2 = (ACCOUNT*)malloc(sizeof(ACCOUNT));
    account2->ref = 6254;
    account2->value = 500;
    strcpy(account2->state, STATE_POSITIVE);
    account2->debit_threshold = 600;

    ACCOUNT* account3 = (ACCOUNT*)malloc(sizeof(ACCOUNT));
    account3->ref = 9456;
    account3->value = 200;
    strcpy(account3->state, STATE_NEGATIVE);
    account3->debit_threshold = 250;

    GSList* accounts = NULL;
    accounts = g_slist_append(accounts, account1);
    accounts = g_slist_append(accounts, account2);
    accounts = g_slist_append(accounts, account3);*/

    //GSList* accounts = read_accounts_from_file();

    // g_slist_foreach() expects a second argument of type GFunc(macro for "void (*)(void *, void*)"), that's why it is being casted
    /*g_slist_foreach(accounts, (GFunc)print_account, NULL);
    g_slist_foreach(accounts, (GFunc)free, NULL);

    g_slist_free(accounts);*/

    /*ACCOUNT* account = get_account(1234);

    print_account(account);

    GSList* accounts = get_accounts();
    GSList* iterator = NULL;

    for(iterator = accounts; iterator; iterator = iterator->next) {
        print_account((ACCOUNT*)iterator->data);
    }

    g_slist_foreach(accounts, (GFunc)free, NULL);
    g_slist_free(accounts);*/

    credit(9456, 100);

    return 0;
}
