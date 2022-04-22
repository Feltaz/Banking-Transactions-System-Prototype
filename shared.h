#ifndef SHARED_H
#define SHARED_H

// NOTE: Beware when allocating arrays, french special characters are 2 bytes long in UTF-8 encoding
extern const char* STATE_POSITIVE;
extern const char* STATE_NEGATIVE;

extern const char* TRANSACTION_TYPE_DEBIT;
extern const char* TRANSACTION_TYPE_CREDIT;

extern const char* TRANSACTION_RESULT_SUCCESS;
extern const char* TRANSACTION_RESULT_FAILURE;

#endif // SHARED_H


// g_slist_foreach() expects a second argument of type GFunc(macro for "void (*)(void *, void*)"), that's why it is being casted
#ifndef free_list
#define free_list(list)\
    g_slist_foreach(list, (GFunc)free, NULL);\
    g_slist_free(list);
#endif // free_list
