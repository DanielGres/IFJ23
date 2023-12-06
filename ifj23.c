// Implementace překladače imperativního jazyka IFJ23.

/**
 * @file ifj23.c
 * @author xgresd00, xmihal13, xkotuc02, xhanco00
 * @brief 
 * @version 0.1
 * @date 2023-12-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "ifj23.h"

int main() {
    // Inicialization of the token tree
    dyn_string string;
    token *T;
    Token_init(&T);
    dynstr_init(&string);
    dynstr_addstr(&string, "START");
    Token_set(&T, &string, startT);
    struct bst_tok_node *the_root;
    the_root = Set_TokNode(T);
    bst_node *symtable;
    Init_BTree(&symtable);

    // Syntax analysis
    if (CorpusPrime(&(the_root->right), &symtable)) {
        // Semantic analysis
        if (Search_BTree_Control(&symtable)) {
           // Code generation
           Generator(the_root, &symtable);
        }
    } else {
        exit(2);
    }
}