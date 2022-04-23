#include <stdio.h>
#include "accounts.h"
#include "transactions.h"
#include "bills.h"

GSList* read_accounts_from_file(){
    char buffer[256];
    ACCOUNT* account = NULL;
    GSList* accounts = NULL;
    FILE* accounts_file = NULL;

    accounts_file = fopen("comptes.txt", "r");

    if(!accounts_file) {
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    fgets(buffer, 256, accounts_file);

    while(!feof(accounts_file)){
        fgets(buffer, 256, accounts_file);

        if(!feof(accounts_file)){
            account = (ACCOUNT*)malloc(sizeof(ACCOUNT));

            sscanf(buffer, "%d %f %s %f", &(account->ref), &(account->value), account->state, &(account->debit_threshold));
            accounts = g_slist_append(accounts, account);
        }
    }

    fclose(accounts_file);

    return accounts;
}

void write_accounts_to_file(GSList* accounts){
    GSList* iterator = NULL;
    ACCOUNT* account = NULL;
    FILE* accounts_file = NULL;

    accounts_file = fopen("comptes.txt", "w");

    if(!accounts_file) {
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    fprintf(accounts_file, "Référence \t\t\t Valeur \t\t\t État \t\t\t Plafond Débit \n");

    for(iterator = accounts; iterator; iterator = iterator->next){
        account = (ACCOUNT*)iterator->data;

        fprintf(accounts_file, "%d \t\t\t %.2f \t\t\t %s \t\t\t %.2f \n", account->ref, account->value, account->state, account->debit_threshold);
    }

    fclose(accounts_file);
}

GSList* get_accounts(){
    return read_accounts_from_file();
}

ACCOUNT* get_account(int ref){
    char buffer[256];
    ACCOUNT temp;
    ACCOUNT* account = NULL;
    FILE* accounts_file = NULL;

    accounts_file = fopen("facture.txt", "r");

    if(!accounts_file){
        printf("Error Occurred While Opening File! \n");
        exit(-1);
    }

    fgets(buffer, 256, accounts_file);

    while(!feof(accounts_file)){
        fgets(buffer, 256, accounts_file);

        if(!feof(accounts_file)){
            sscanf(buffer, "%d %f %s %f", &(temp.ref), &(temp.value), temp.state, &(temp.debit_threshold));

            if(temp.ref == ref){
                account = (ACCOUNT*)malloc(sizeof(ACCOUNT));

                *account = temp;
                break;
            }
        }
    }

    fclose(accounts_file);

    return account;
}

int debit(int ref, float value){
    GSList* iterator = NULL;
    ACCOUNT* temp = NULL;
    ACCOUNT* account = NULL;

    GSList* accounts = read_accounts_from_file();

    for(iterator = accounts; iterator != NULL; iterator = iterator->next){
        temp = (ACCOUNT*)iterator->data;

        if(temp->ref == ref){
            account = temp;
            break;
        }
    }

    if(!account){
        printf("Account with reference %d was not found! \n", ref);
        free_list(accounts);

        return -1;
    }

    if(!strncmp(account->state, STATE_POSITIVE, 9)){
        if(account->value >= value){
            account->value -= value;

            add_transaction(account->ref, TRANSACTION_TYPE_DEBIT, value, TRANSACTION_RESULT_SUCCESS, account->state);
        }
        else if(account->value < value && value - account->value <= account->debit_threshold){
            strncpy(account->state, STATE_NEGATIVE, 9);
            account->value = value - account->value;

            add_transaction(account->ref, TRANSACTION_TYPE_DEBIT, value, TRANSACTION_RESULT_SUCCESS, account->state);
            increment_bill(account->ref, account->value);
        }
        else if(account->value < value && value - account->value > account->debit_threshold){
            printf("Transaction failed! Amount exceeded negative withdrawal threshold. \n");
            add_transaction(account->ref, TRANSACTION_TYPE_DEBIT, value, TRANSACTION_RESULT_FAILURE, account->state);

            free_list(accounts);
            return -2;
        }
    }
    else if(!strncmp(account->state, STATE_NEGATIVE, 9)){
        if(account->value + value <= account->debit_threshold){
            account->value += value;

            add_transaction(account->ref, TRANSACTION_TYPE_DEBIT, value, TRANSACTION_RESULT_SUCCESS, account->state);
            increment_bill(account->ref, value);
        }
        else if(account->value + value > account->debit_threshold){
            printf("Transaction failed! Amount exceeded negative withdrawal threshold. \n");
            add_transaction(account->ref, TRANSACTION_TYPE_DEBIT, value, TRANSACTION_RESULT_FAILURE, account->state);

            free_list(accounts);
            return -2;
        }
    }

    write_accounts_to_file(accounts);
    free_list(accounts);

    return 0;
}

int credit(int ref, float value){
    GSList* iterator = NULL;
    ACCOUNT* temp = NULL;
    ACCOUNT* account = NULL;
    GSList* accounts = read_accounts_from_file();

    for(iterator = accounts; iterator != NULL; iterator = iterator->next){
        temp = (ACCOUNT*)iterator->data;

        if(temp->ref == ref){
            account = temp;
            break;
        }
    }

    if(!account){
        printf("Account with reference %d was not found! \n", ref);
        free_list(accounts);

        return -1;
    }

    if(!strncmp(account->state, STATE_POSITIVE, 9)){
        account->value += value;

        add_transaction(account->ref, TRANSACTION_TYPE_CREDIT, value, TRANSACTION_RESULT_SUCCESS, account->state);
    }
    else if(!strncmp(account->state, STATE_NEGATIVE, 9)){
        if(account->value - value <= 0){
            strncpy(account->state, STATE_POSITIVE, 9);
            account->value = value - account->value;

            add_transaction(account->ref, TRANSACTION_TYPE_CREDIT, value, TRANSACTION_RESULT_SUCCESS, account->state);
        }
        else if(account->value - value > 0){
            account->value -= value;

            add_transaction(account->ref, TRANSACTION_TYPE_CREDIT, value, TRANSACTION_RESULT_SUCCESS, account->state);
        }
    }

    write_accounts_to_file(accounts);
    free_list(accounts);

    return 0;
}


