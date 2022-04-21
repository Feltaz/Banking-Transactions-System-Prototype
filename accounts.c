#include <stdio.h>
#include "accounts.h"
#include "shared.h"

GSList* read_accounts_from_file(){
    char buffer[250];
    ACCOUNT* data = NULL;
    GSList* accounts = NULL;
    FILE* accounts_file = fopen("comptes.txt", "r");

    if(!accounts_file) {
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

    fclose(accounts_file);

    return accounts;
}

void write_accounts_to_file(GSList* accounts){
    GSList* iterator = NULL;
    ACCOUNT* iterator_data = NULL;
    FILE* accounts_file = fopen("comptes.txt", "w");

    if(!accounts_file) {
        printf("Error Occurred While Opening File!");
        exit(-1);
    }

    fprintf(accounts_file, "Référence \t\t\t Valeur \t\t\t État \t\t\t Plafond Débit \n");

    for(iterator = accounts; iterator; iterator = iterator->next){
        iterator_data = (ACCOUNT*)iterator->data;

        fprintf(accounts_file, "%d \t\t\t %d \t\t\t %s \t\t\t %d \n", iterator_data->ref, iterator_data->value, iterator_data->state, iterator_data->debit_threshold);
    }

    fclose(accounts_file);
}

GSList* get_accounts(){
    return read_accounts_from_file();
}

ACCOUNT* get_account(int ref){
    GSList* iterator = NULL;
    ACCOUNT* iterator_data = NULL;
    ACCOUNT* result = NULL;
    GSList* accounts = read_accounts_from_file();

    for(iterator = accounts; iterator != NULL; iterator = iterator->next){
        iterator_data = (ACCOUNT*)iterator->data;

        if(iterator_data->ref == ref){
            result = (ACCOUNT*)malloc(sizeof(ACCOUNT));

            if(!result){
                printf("Error! Could not allocate memory! \n");
                exit(-1);
            }

            *result = *iterator_data;
            break;
        }
    }

    free_list(accounts);

    return result;
}

int debit(int ref, int value){
    GSList* iterator = NULL;
    ACCOUNT* iterator_data = NULL;
    ACCOUNT* account = NULL;

    GSList* accounts = read_accounts_from_file();

    for(iterator = accounts; iterator != NULL; iterator = iterator->next){
        iterator_data = (ACCOUNT*)iterator->data;

        if(iterator_data->ref == ref){
            account = iterator_data;
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
        }
        else if(account->value < value && value - account->value <= account->debit_threshold){
            strncpy(account->state, STATE_NEGATIVE, 9);
            account->value = value - account->value;
        }
        else if(account->value < value && value - account->value > account->debit_threshold){
            printf("Transaction failed! Amount exceeded negative withdrawal threshold.");
            free_list(accounts);

            return -1;
        }
    }
    else if(!strncmp(account->state, STATE_NEGATIVE, 9)){
        if(account->value + value <= account->debit_threshold){
            account->value += value;
        }
        else if(account->value + value > account->debit_threshold){
            printf("Transaction failed! Amount exceeded negative withdrawal threshold.");
            free_list(accounts);

            return -1;
        }
    }

    write_accounts_to_file(accounts);
    free_list(accounts);

    return 0;
}

int credit(int ref, int value){
    GSList* iterator = NULL;
    ACCOUNT* iterator_data = NULL;
    ACCOUNT* account = NULL;
    GSList* accounts = read_accounts_from_file();

    for(iterator = accounts; iterator != NULL; iterator = iterator->next){
        iterator_data = (ACCOUNT*)iterator->data;

        if(iterator_data->ref == ref){
            account = iterator_data;
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
    }
    else if(!strncmp(account->state, STATE_NEGATIVE, 9)){
        if(account->value - value <= 0){
            strncpy(account->state, STATE_POSITIVE, 9);
            account->value = value - account->value;
        }
        else if(account->value - value > 0){
            account->value -= value;
        }
    }

    write_accounts_to_file(accounts);
    free_list(accounts);

    return 0;
}


