#include <stdio.h>
#include "transactions.h"

GSList* read_transactions_from_file(){
    char buffer[256];
    TRANSACTION* transaction = NULL;
    GSList* transactions = NULL;
    FILE* transactions_file = NULL;

    transactions_file = fopen("histo.txt", "r");

    if(!transactions_file) {
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fgets(buffer, 256, transactions_file);

    while(!feof(transactions_file)){
        fgets(buffer, 256, transactions_file);
        if(!feof(transactions_file)){
            transaction = (TRANSACTION*)malloc(sizeof(TRANSACTION));

            sscanf(buffer, "%d %s %d %s %s", &(transaction->ref), transaction->transaction, &(transaction->value), transaction->result, transaction->state);
            transactions = g_slist_append(transactions, transaction);
        }
    }

    fclose(transactions_file);

    return transactions;
}

void write_transactions_to_file(GSList* transactions){
    GSList* iterator = NULL;
    TRANSACTION* transaction = NULL;
    FILE* transactions_file = NULL;

    transactions_file = fopen("histo.txt", "w");

    if(!transactions_file) {
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fprintf(transactions_file, "Référence \t\t\t Transaction \t\t\t Valeur \t\t\t Résultat \t\t\t État \n");

    for(iterator = transactions; iterator; iterator = iterator->next){
        transaction = (TRANSACTION*)iterator->data;

        fprintf(transactions_file, "%d \t\t\t %s \t\t\t %d \t\t\t %s \t\t\t %s \n", transaction->ref, transaction->transaction, transaction->value, transaction->result, transaction->state);
    }

    fclose(transactions_file);
}

GSList* get_transactions(){
    return read_transactions_from_file();
}

int add_transaction(int ref, char* transaction, int value, char* result, char* state){
    FILE* transactions_file = fopen("histo.txt", "a");

    if(!transactions_file){
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fprintf(transactions_file, "%d \t\t\t %s \t\t\t %d \t\t\t %s \t\t\t %s \n", ref, transaction, value, result, state);

    fclose(transactions_file);

    return 0;
}

