#include "codegen.h"

void Instructions(){
    printf(
        "LABEL READSTRING\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval string\n"
        "POPFRAME\n"
        "RETURN\n"
        );
    
    printf(
        "LABEL READINT\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval int\n"
        "POPFRAME\n"
        "RETURN\n"
        );

    printf(
        "LABEL READDOUBLE\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval double\n"
        "POPFRAME\n"
        "RETURN\n"
        );

    printf(
        "LABEL READBOOL\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval bool\n"
        "POPFRAME\n"
        "RETURN\n"
        );
    
    printf(
        "LABEL WRITE\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@writeval\n"
        "POPS LF@writeval\n"
        "WRITE LF@writeval\n"
        "POPFRAME\n"
        "RETURN\n"
        );
}


void GenerateSubTree(struct bst_tok_node *curr_root){
    if(curr_root == NULL) return;
    printf("%d ",curr_root->T->dtype);
    GenerateSubTree(curr_root->right);
}

void Generator(struct bst_tok_node *root){
    //Instructions();
    GenerateSubTree((root->right));

}