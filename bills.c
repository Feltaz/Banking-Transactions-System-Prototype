#include <stdio.h>
#include "shared.h"
#include "bills.h"

pthread_mutex_t bills_file_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for bills file

GSList* read_bills_from_file(){
    char buffer[256]; // Buffer to store lines read from file
    BILL* bill = NULL;
    GSList* bills = NULL;
    FILE* bills_file = NULL;

    bills_file = fopen("facture.txt", "r");

    if(!bills_file) {
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

     // Read line from file
    fgets(buffer, 256, bills_file);

    while(!feof(bills_file)){
        fgets(buffer, 256, bills_file);

        // Check again for EOF to avoid reading the last blank line
        if(!feof(bills_file)){
            bill = (BILL*)malloc(sizeof(BILL));

            sscanf(buffer, "%d %f", &(bill->ref), &(bill->debt));
            bills = g_slist_append(bills, bill);
        }
    }

    fclose(bills_file);

    return bills;
}

void write_bills_to_file(GSList* bills){
    GSList* iterator = NULL; // Iterator for linked list
    BILL* bill = NULL; // Buffer to hold the bill in a linked list node
    FILE* bills_file = NULL;

    bills_file = fopen("facture.txt", "w");

    if(!bills_file) {
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    fprintf(bills_file, "Référence \t\t\t Somme à payer \n");

    for(iterator = bills; iterator; iterator = iterator->next){
        bill = (BILL*)iterator->data;

        fprintf(bills_file, "%d \t\t\t %.2f \n", bill->ref, bill->debt);
    }

    fclose(bills_file);
}

GSList* get_bills(){
    GSList* bills = NULL;

    pthread_mutex_lock(&bills_file_mutex);
    bills = read_bills_from_file();
    pthread_mutex_unlock(&bills_file_mutex);

    return bills;
}

int increment_bill(int ref, float debit_value){
    GSList* iterator = NULL; // Iterator for linked list
    BILL* temp = NULL; // Buffer to hold the bill in a linked list node
    BILL* bill = NULL;

    pthread_mutex_lock(&bills_file_mutex);
    // Load bills to memory
    GSList* bills = read_bills_from_file();

    for(iterator = bills; iterator != NULL; iterator = iterator->next){
        temp = (BILL*)iterator->data;

        if(temp->ref == ref){
            bill = temp;
            break;
        }
    }

    // If no bill exists for the account, create one
    if(!bill){
        bill = (BILL*)malloc(sizeof(BILL));
        bill->ref = ref;
        bill->debt = (debit_value * 2)/100;

        bills = g_slist_append(bills, bill);
    }
    // If a bill exists for the account, increment it
    else {
        bill->debt += (debit_value * 2)/100;
    }

    // Save bills to file
    write_bills_to_file(bills);
    pthread_mutex_unlock(&bills_file_mutex);
    free_list(bills);

    return 0;
}

BILL* get_bill(int ref){
    char buffer[256]; // Buffer to store lines read from file
    BILL temp;
    BILL* bill = NULL;
    FILE* bills_file = NULL;

    pthread_mutex_lock(&bills_file_mutex);
    bills_file = fopen("facture.txt", "r");

    if(!bills_file){
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    // Read line from file
    fgets(buffer, 256, bills_file);

    // Check again for EOF to avoid reading the last blank line
    while(!feof(bills_file)){
        fgets(buffer, 256, bills_file);

        if(!feof(bills_file)){
            sscanf(buffer, "%d %f", &(temp.ref), &(temp.debt));

            if(temp.ref == ref){
                bill = (BILL*)malloc(sizeof(BILL));

                *bill = temp;
                break;
            }
        }
    }

    fclose(bills_file);
    pthread_mutex_unlock(&bills_file_mutex);

    return bill;
}
