#include <stdio.h>
#include "transactions.h"

pthread_mutex_t transactions_file_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for transactions file

GSList* read_transactions_from_file(){
    char buffer[256]; // Buffer to store lines read from file
    TRANSACTION* transaction = NULL;
    GSList* transactions = NULL;
    FILE* transactions_file = NULL;

    transactions_file = fopen("histo.txt", "r");

    if(!transactions_file) {
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    // Read line from file
    fgets(buffer, 256, transactions_file);

    // Check again for EOF to avoid reading the last blank line
    while(!feof(transactions_file)){
        fgets(buffer, 256, transactions_file);
        if(!feof(transactions_file)){
            transaction = (TRANSACTION*)malloc(sizeof(TRANSACTION));

            sscanf(buffer, "%d %s %f %s %s", &(transaction->ref), transaction->transaction, &(transaction->value), transaction->result, transaction->state);
            transactions = g_slist_append(transactions, transaction);
        }
    }

    fclose(transactions_file);

    return transactions;
}

void write_transactions_to_file(GSList* transactions){
    GSList* iterator = NULL; // Iterator for linked list
    TRANSACTION* transaction = NULL; // Buffer to hold the transaction in a linked list node
    FILE* transactions_file = NULL;

    transactions_file = fopen("histo.txt", "w");

    if(!transactions_file) {
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    fprintf(transactions_file, "Référence \t\t\t Transaction \t\t\t Valeur \t\t\t Résultat \t\t\t État \n");

    for(iterator = transactions; iterator; iterator = iterator->next){
        transaction = (TRANSACTION*)iterator->data;

        fprintf(transactions_file, "%d \t\t\t %s \t\t\t %.2f \t\t\t %s \t\t\t %s \n", transaction->ref, transaction->transaction, transaction->value, transaction->result, transaction->state);
    }

    fclose(transactions_file);
}

GSList* get_transactions(){
    GSList* transactions = NULL;

    pthread_mutex_lock(&transactions_file_mutex);
    read_transactions_from_file();
    pthread_mutex_unlock(&transactions_file_mutex);

    return transactions;
}

int add_transaction(int ref, const char* transaction, float value, const char* result, const char* state){
    pthread_mutex_lock(&transactions_file_mutex);
    FILE* transactions_file = fopen("histo.txt", "a");

    if(!transactions_file){
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    // Append transaction to file
    fprintf(transactions_file, "%d \t\t\t %s \t\t\t %.2f \t\t\t %s \t\t\t %s \n", ref, transaction, value, result, state);

    fclose(transactions_file);
    pthread_mutex_unlock(&transactions_file_mutex);

    return 0;
}

