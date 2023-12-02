// Implementace překladače imperativního jazyka IFJ22
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#include "symtable.h"

#define STRMALCPY(to, from) strcpy(to = malloc(strlen(from) + 1), from)

bool InArray(char *str2) {
    char arr[11][12] = {"readString", "readInt", "readDouble", "write", "Int2Double", "Double2Int", "length", "substring", "substring", "ord", "chr"};
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
        (*root)->funcTree = NULL;
        if (var_type) {
            if (!declaration) {
                // printf("Chyba pouzita premenna nie je deklarovana\n");
                exit(5);
            }
            (*root)->var_declared = declaration;
            (*root)->variable = true;
            (*root)->function = false;
        } else {
            (*root)->func_declared = declaration;
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
                        printf("Chyba Anton\n");
                        exit(3);
                    }
                } else {
                    if (!(*root)->var_declared) {
                        printf("Chyba Bernolak\n");
                        exit(3);
                    } else {
                        (*root)->variable = true;
                    }
                }
            } else {
                if (declaration) {
                    if (!(*root)->var_declared) {
                        (*root)->var_declared = declaration;
                    } else {
                        exit(3);
                        // printf("Chyba\n");
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

bool Search_BTree_Control(bst_node **root) {
    if ((*root) != NULL) {
        // printf("Name = %s IsVar: %d declared: %d | Isfunc: %d declared: %d\n", (*root)->name, (*root)->variable, (*root)->var_declared, (*root)->function, (*root)->func_declared);
        if ((*root)->function == true) {
            if (!(InArray((*root)->name))) {
                if (!((*root)->func_declared)) {
                    // 3
                    exit(3);
                    return false;
                }
            } else {
                if (((*root)->func_declared)) {
                    exit(3);
                    return false;
                }
            }

        } else if ((*root)->variable == true) {
            if ((!(*root)->var_declared)) {
                // exit(5);
                return false;
            }
        }
        return (Search_BTree_Control(&((*root)->left)) || Search_BTree_Control(&((*root)->right)));
    }
    return true;
}
bool Is_In_BTree(bst_node **root, char *name) {
    if ((*root) != NULL) {
        if (strcmp((*root)->name, name) == 0) {
            return true;
        }
        return (Is_In_BTree(&((*root)->left), name) || Is_In_BTree(&((*root)->right), name));
    }
    return false;
}
bool Insert_FuncSubTree(bst_node **root, char *funcName, char *newName, token_type type, bool declaration, bool var_type) {
    if ((*root) != NULL) {
        if (strcmp((*root)->name, funcName) == 0) {
            return Insert_BTree(&(*root)->funcTree, newName, type, declaration, var_type);
        }
        return (Insert_FuncSubTree(&((*root)->left), funcName, newName, type, declaration, var_type) || Insert_FuncSubTree(&((*root)->right), funcName, newName, type, declaration, var_type));
    }
    return false;
}

struct bst_node *SubTreePointer(bst_node *root, char *name) {
    bst_node *ptr = root;
    while (ptr != NULL) {
        if (strcmp(ptr->name, name) == 0) {
            return ptr;
        } else if (strcmp(name, ((root)->name)) > 0) {
            ptr = ptr->left;
        } else if (strcmp(name, ((root)->name)) < 0) {
            ptr = ptr->right;
        }
    }
}

void PrintAllVariablesinScope(bst_node **root, bool GF) {
    if ((*root) == NULL)
        return;

    else {
        if ((*root)->variable) {
            if (GF) {
                printf("DEFVAR GF@%s\n", (*root)->name);
            } else {
                printf("DEFVAR LF@%s\n", (*root)->name);
            }
        }
        // printf("Name = %s IsVar: %d declared: %d | Isfunc: %d declared: %d\n", (*root)->name, (*root)->variable, (*root)->var_declared, (*root)->function, (*root)->func_declared);
        PrintAllVariablesinScope(&(*root)->left, GF);
        PrintAllVariablesinScope(&(*root)->right, GF);
    }
}