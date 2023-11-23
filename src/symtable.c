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
            (*root)->var_declared = declaration;
            (*root)->variable = true;
        } else {
            (*root)->func_declared = declaration;
            (*root)->function = true;
        }
        return true;
    } else {
        if (strcmp(name, ((*root)->name)) > 0) {
            return Insert_BTree(&(*root)->left, name, type, declaration, var_type);
        } else if (strcmp(name, ((*root)->name)) < 0) {
            return Insert_BTree(&(*root)->right, name, type, declaration, var_type);
        } else {
            if (!(*root)->func_declared) {
                (*root)->func_declared = declaration;
            } else {
                // ERROR 3
            }
            return true;
        }
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
    printf("Name = %s declared: %d", (*root)->name, (*root)->func_declared);
    preorderTraversal(&(*root)->left);
    preorderTraversal(&(*root)->right);
}

bool Search_BTree(bst_node **root) {
    if ((*root) != NULL) {
        if ((*root)->function = true) {
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
        } else {
            return (Search_BTree(&((*root)->left)) && Search_BTree(&((*root)->right)));
        }
    }
    return true;
}
