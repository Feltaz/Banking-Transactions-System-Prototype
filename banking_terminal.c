#include <stdio.h>
#include "banking_terminal.h"
#include "transactions.h"
#include "accounts.h"
#include "bills.h"

void print_account(gpointer item){
    ACCOUNT* account = (ACCOUNT*)item;

    printf("Référence: %d\n", account->ref);
    printf("Valeur: %.2f\n", account->value);
    printf("État: %s\n", account->state);
    printf("Plafond Débit: %.2f\n", account->debit_threshold);
    printf("\n");
}

void print_transaction(gpointer item){
    TRANSACTION* transaction = (TRANSACTION*)item;

    printf("Référence: %d\n", transaction->ref);
    printf("Transaction: %s\n", transaction->transaction);
    printf("Valeur: %.2f\n", transaction->value);
    printf("Résultat: %s\n", transaction->result);
    printf("État: %s\n", transaction->state);
    printf("\n");
}

void print_bill(gpointer item){
    BILL* bill = (BILL*)item;

    printf("Référence: %d\n", bill->ref);
    printf("Somme à payer: %.2f\n", bill->debt);
    printf("\n");
}

void* main_banking_menu(void* arg) {
    char choice;
    int ref;

    while(1){
        system("clear");

        printf("----------------------------------------------------------------------------------\n");
        printf("================================== Terminal Banque ===============================\n");
        printf("----------------------------------------------------------------------------------\n");
        printf("                              Veuillez choisir une option                         \n");
        printf("                                                                                  \n");
        printf("                             1 - Afficher comptes                                 \n");
        printf("                             2 - Afficher compte par référence                    \n");
        printf("                             3 - Afficher factures                                \n");
        printf("                             4 - Afficher facture par référence                   \n");
        printf("                             5 - Afficher transactions                            \n");
        printf("                             6 - Quitter                                          \n");
        printf("----------------------------------------------------------------------------------\n");

        do{
            scanf("%c", &choice);
        }
        while(choice != '1' && choice != '2' && choice != '3' && choice != '4' && choice != '5' && choice != '6');

         if(choice == '6'){
            system("clear");
            break;
        }

        system("clear");

        switch(choice){
        case '1':
            printf("La liste des comptes: \n");

            GSList* accounts = get_accounts();
            g_slist_foreach(accounts, (GFunc)print_account, NULL);
            g_slist_foreach(accounts, (GFunc)free, NULL);
            g_slist_free(accounts);

            printf("Appuyez sur un bouton pour continuer... \n");
            getchar();
            getchar();
            break;
        case '2':
            printf("Saisir la référence du compte: ");
            scanf("%d", &ref);

            ACCOUNT* account = get_account(ref);
            if(account == NULL){
                printf("Le compte n'existe pas! \n");
            }
            else {
                print_account((gpointer) account);
                free(account);
            }

            printf("Appuyez sur un bouton pour continuer... \n");
            getchar();
            getchar();
            break;
        case '3':
            printf("La liste des factures: \n");

            GSList* bills = get_bills();
            g_slist_foreach(bills, (GFunc)print_bill, NULL);
            g_slist_foreach(bills, (GFunc)free, NULL);
            g_slist_free(bills);

            printf("Appuyez sur un bouton pour continuer... \n");
            getchar();
            getchar();
            break;
        case '4':
            printf("Saisir la référence du compte: ");
            scanf("%d", &ref);

            BILL* bill = get_bill(ref);
            if(bill == NULL){
                printf("Le compte n'existe pas! \n");
            }
            else {
                print_bill((gpointer) bill);
                free(bill);
            }

            printf("Appuyez sur un bouton pour continuer... \n");
            getchar();
            getchar();
            break;
        case '5':
            printf("La liste des transactions: \n");

            GSList* transactions = get_transactions();
            g_slist_foreach(transactions, (GFunc)print_transaction, NULL);
            g_slist_foreach(transactions, (GFunc)free, NULL);
            g_slist_free(transactions);

            printf("Appuyez sur un bouton pour continuer... \n");
            getchar();
            getchar();
            break;
        }
    }

    return NULL;
}
