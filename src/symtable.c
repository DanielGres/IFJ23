// Implementace překladače imperativního jazyka IFJ22
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#include "../include/symtable.h"

#define STRMALCPY(to, from) strcpy(to = malloc(strlen(from) + 1), from)

bool InArray(char *str2) {
    char arr[11][10] = {"reads", "readInt", "readf", "write", "floatval", "intval", "strval", "strlen", "substring", "ord", "chr"};
    for (int i = 0; i < 11; i++) {
        if (strcmp(arr[i], str2) == 0) {
            return true;
        }
    }
    return false;
}

void Init_BTree(bst_node **root) {
    (*root) = NULL;
}
bool Insert_BTree(bst_node **root, char *name, token_type type, bool declaration, bool var_type) {
    if ((*root) == NULL) {
        (*root) = malloc(sizeof(bst_node));
        if ((*root) == NULL) {
            return false;
        }
        STRMALCPY(((*root)->name), name);
        (*root)->type = type;
        (*root)->left = NULL;
        (*root)->right = NULL;
        if (var_type) {
            if (!declaration) {
                printf("Chyba pouzita premenna nie je deklarovana\n");
            }
            (*root)->var_declared = declaration;
            (*root)->variable = true;
            (*root)->function = false;
        } else {
            (*root)->func_declared = true;
            (*root)->variable = false;
            (*root)->function = true;
        }
        return true;
    } else {
        if (strcmp(name, ((*root)->name)) > 0) {
            return Insert_BTree(&(*root)->left, name, type, declaration, var_type);
        } else if (strcmp(name, ((*root)->name)) < 0) {
            return Insert_BTree(&(*root)->right, name, type, declaration, var_type);
        } else {
            // IS VARIABLE
            if (var_type) {
                if (declaration) {
                    if (!(*root)->var_declared) {
                        (*root)->var_declared = declaration;
                    } else {
                        printf("Chyba\n");
                    }
                } else {
                    if (!(*root)->variable) {
                        printf("Chyba\n");
                    } else {
                        (*root)->variable = true;
                    }
                }
            } else {
                if (declaration) {
                    if (!(*root)->var_declared) {
                        (*root)->var_declared = declaration;
                    } else {
                        printf("Chyba\n");
                    }
                } else {
                    (*root)->function = true;
                }
            }
        }
        return true;
    }
    return false;
}
void preorderTraversal(bst_node **root) {
    if ((*root) == NULL)
        return;
    // if ((*root)->type == variableT)
    // {
    //     printf("DEFVAR LF@%s\n", (*root)->name);
    // }
    printf("Name = %s IsVar: %d declared: %d | Isfunc: %d declared: %d\n", (*root)->name, (*root)->variable, (*root)->var_declared, (*root)->function, (*root)->func_declared);
    preorderTraversal(&(*root)->left);
    preorderTraversal(&(*root)->right);
}

bool Search_BTree(bst_node **root) {
    if ((*root) != NULL) {
       // printf("Name = %s IsVar: %d declared: %d | Isfunc: %d declared: %d\n", (*root)->name, (*root)->variable, (*root)->var_declared, (*root)->function, (*root)->func_declared);
        if ((*root)->function == true) {
            if (!(InArray((*root)->name))) {
                if (!((*root)->func_declared)) {
                    // 3
                    return false;
                }
            } else {
                if (((*root)->func_declared)) {
                    // 3
                    return false;
                }
            }

        } else if ((*root)->variable == true) {
            if ((!(*root)->var_declared)) {
                // 3
                return false;
            }

        }
            return (Search_BTree(&((*root)->left)) && Search_BTree(&((*root)->right)));
    }
    return true;
}
