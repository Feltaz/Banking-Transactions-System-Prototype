#include <stdio.h>
#include "bills.h"

GSList* read_bills_from_file(){
    char buffer[256];
    BILL* bill = NULL;
    GSList* bills = NULL;
    FILE* bills_file = NULL;

    bills_file = fopen("facture.txt", "r");

    if(!bills_file) {
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fgets(buffer, 256, bills_file);

    while(!feof(bills_file)){
        fgets(buffer, 256, bills_file);

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
    GSList* iterator = NULL;
    BILL* bill = NULL;
    FILE* bills_file = NULL;

    bills_file = fopen("facture.txt", "w");

    if(!bills_file) {
        printf("Error Occurred While Opening File!");
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
    return read_bills_from_file();
}

int add_bill(int ref, float debt){
    FILE* bills_file = fopen("facture.txt", "a");

    if(!bills_file){
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fprintf(bills_file, "%d \t\t\t %.2f \n", ref, debt);

    fclose(bills_file);

    return 0;
}

BILL* get_bill(int ref){
    char buffer[256];
    BILL temp;
    BILL* bill = NULL;
    FILE* bills_file = NULL;

    bills_file = fopen("facture.txt", "r");

    if(!bills_file){
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fgets(buffer, 256, bills_file);

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

    return bill;
}
