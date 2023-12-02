// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#include "ifj23.h"

// TODO na lexer testovanie dik pls nechaj sem abo zakomentuj lem
const char *enumers[] = {
    "varidT",
    "funcidT",
    "operatorT",
    "colonT",
    "vartypeT",
    "vartypeQT",
    "equalT",
    "intnumT",
    "doublenumT",
    "varT",
    "letT",
    "ifT",
    "stringT",
    "nilT",
    "LbracketT",
    "RbracketT",
    "LCbracketT",
    "RCbracketT",
    "elseT",
    "funcT",
    "paramNameT",
    "commaT",
    "arrowT",
    "whileT",
    "returnT",
    "newlineT",
    "semicolonT",
    "linecommentT",
    "blockcommentT",
    "eofT",
    "startT",
    "helpT",
    "underscoreT"
};

int main() {
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

    // TODO lexer testing potom vymazat
    // while(Get_Token(&T)){
    //     dynstr_print(T->val);
    //     printf(" %s\n",enumers[T->dtype]);
    // }

    if (CorpusPrime(&(the_root->right), &symtable)) {
        // PostorderTraversal(the_root);
        if (Search_BTree_Control(&symtable)) {
            // printf("Spravne ;) :fire: \n");
            Generator(the_root, &symtable);
        }
        //bst_node *ptr = SubTreePointer(symtable, "kokos");
        // inorderTraversal(the_root);
        // printf("\n");
        // preorderTraversal(&symtable);
        //preorderTraversal(&ptr->funcTree);
        //PrintAllVariablesinScope(&symtable, true);
        //PrintAllVariablesinScope(&(ptr)->funcTree, false);
    } else {
        exit(2);
    }
}