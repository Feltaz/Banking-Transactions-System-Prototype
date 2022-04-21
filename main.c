#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

// NOTE: Beware when allocating arrays, french special characters are 2 bytes long in UTF-8 encoding
const char* STATE_POSITIVE = "Positif";
const char* STATE_NEGATIVE = "Négatif";
const char* TRANSACTION_TYPE_DEBIT = "Débit";
const char* TRANSACTION_TYPE_CREDIT = "Crédit";
const char* TRANSACTION_RESULT_SUCCESS = "Succès";
const char* TRANSACTION_RESULT_FAILURE = "Échec";

typedef struct {
    int ref;
    int value;
    char state[9];
    int debit_threshold;
} ACCOUNT;

typedef struct {
    int ref;
    int value;
    char transaction[8];
    char result[8];
    char state[9];
} TRANSACTION;

typedef struct {
    int ref;
    float debt;
} BILLING;

void print_account(gpointer item){
    ACCOUNT account = *((ACCOUNT*)item);

    printf("Account Details: \n");
    printf("Reference: %d\n", account.ref);
    printf("Value: %d\n", account.value);
    printf("State: %s\n", account.state);
    printf("Debit Threshold: %d\n", account.debit_threshold);
    printf("\n");
}

GSList* read_accounts_from_file(){
    char buffer[250];
    ACCOUNT* data = NULL;
    GSList* accounts = NULL;
    FILE* accounts_file = fopen("comptes.txt", "r");

    if(accounts_file == NULL) {
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fgets(buffer, 250, accounts_file);

    while(!feof(accounts_file)){
        fgets(buffer, 250, accounts_file);
        if(!feof(accounts_file)){
            data = (ACCOUNT*)malloc(sizeof(ACCOUNT));

            sscanf(buffer, "%d %d %s %d", &(data->ref), &(data->value), data->state, &(data->debit_threshold));
            accounts = g_slist_append(accounts, data);
        }
    }

    return accounts;
}

void write_accounts_to_file(GSList* accounts){
    GSList* iterator = NULL;
    ACCOUNT* iterator_data = NULL;
    FILE* accounts_file = fopen("comptes.txt", "w");

    if(accounts_file == NULL) {
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fprintf(accounts_file, "Référence \t\t\t Valeur \t\t\t État \t\t\t Plafond Débit \n");

    for(iterator = accounts; iterator != NULL; iterator = iterator->next){
        iterator_data = (ACCOUNT*)iterator->data;

        fprintf(accounts_file, "%d \t\t\t %d \t\t\t %s \t\t\t %d \n", iterator_data->ref, iterator_data->value, iterator_data->state, iterator_data->debit_threshold);
    }
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

    GSList* accounts = read_accounts_from_file();

    // g_slist_foreach() expects a second argument of type GFunc(macro for "void (*)(void *, void*)"), that's why it is being casted
    g_slist_foreach(accounts, (GFunc)print_account, NULL);
    g_slist_foreach(accounts, (GFunc)free, NULL);

    g_slist_free(accounts);

    return 0;
}
