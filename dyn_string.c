// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#include "dyn_string.h"

#define STRING_ALLOC_SIZE 8  // Magic number

void dynstr_add(dyn_string *str, char c) {
    if (str->len + 1 >= str->allen) {
        str->allen += STRING_ALLOC_SIZE;
        str->s = (char *)realloc(str->s, str->allen);
    }
    str->s[str->len++] = c;
    str->s[str->len] = '\0';
}

void dynstr_addstr(dyn_string *str, char *strtoadd) {
    for (int i = 0; i < strlen(strtoadd); i++) {
        dynstr_add(str, strtoadd[i]);
    }
}

void dynstr_init(dyn_string *str) {
    if (str != NULL) {
        str->s = (char *)malloc(STRING_ALLOC_SIZE);
        str->len = 0;
        str->allen = STRING_ALLOC_SIZE;
        str->s[str->len] = '\0';
    } else {
        printf("Internal error: dynstr_init failed to allocate memory.\n");
        exit(99);
    }
}

void dynstr_copy(dyn_string *to, dyn_string *from) {
    if (from->len >= to->allen) {
        to->allen = from->len + 1;
        to->s = (char *)realloc(to->s, to->allen);
    }
    strcpy(to->s, from->s);
    to->len = from->len;
}

bool dynstr_cmp(dyn_string *string1, char *string2) {
    // printf("String1 -> %s\n", string1->s);
    // printf("String2 -> %s\n", string2);
    return (strcmp(string1->s, string2) == 0);
}

void dynstr_clear(dyn_string *str) {
    str->len = 0;
    str->s[str->len] = '\0';
}

void dynstr_destr(dyn_string *str) {
    str->allen = 0;
    str->len = 0;
    free(str->s);
}
void dynstr_print(dyn_string *str) {
    for (int i = 0; i < str->len; i++) {
        printf("%c", str->s[i]);
    }
}