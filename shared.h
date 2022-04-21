#ifndef CONSTANTS_H
#define CONSTANTS_H

// NOTE: Beware when allocating arrays, french special characters are 2 bytes long in UTF-8 encoding
const char* STATE_POSITIVE = "Positif";
const char* STATE_NEGATIVE = "Négatif";

const char* TRANSACTION_TYPE_DEBIT = "Débit";
const char* TRANSACTION_TYPE_CREDIT = "Crédit";

const char* TRANSACTION_RESULT_SUCCESS = "Succès";
const char* TRANSACTION_RESULT_FAILURE = "Échec";

#endif // CONSTANTS_H


// g_slist_foreach() expects a second argument of type GFunc(macro for "void (*)(void *, void*)"), that's why it is being casted
#ifndef free_list
#define free_list(list)\
    g_slist_foreach(list, (GFunc)free, NULL);\
    g_slist_free(list);
#endif // free_list
