#include "ifj23.h"

int main() {

    dyn_string string;
    token *T;
    Token_init(&T);
    dynstr_init(&string);
    dynstr_addstr(&string, "START");
    Token_set(&T, &string, startT);
    struct bst_tok_node *the_root;
    the_root = Set_TokNode(T);
    
    if(CorpusPrime(&(the_root->right))){
        printf("spravne! :DD\n");
        Generator(the_root);
    }
    else{
        printf("nespravne! :((\n");
    }



}
