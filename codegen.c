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
    switch(curr_root->T->dtype){
        case letT:{
            printf("DEFVAR GF@mylet\n");
            printf("MOVE GF@mylet somevar\n");
        }
        break;
        case ifT:{
            printf("IF\n");
            GenerateIF(curr_root->left);
        }
        break;
        default:{
            exit (0);
        }
    }
    GenerateSubTree(curr_root->right);
}

void Generator(struct bst_tok_node *root){
    //Instructions();
    GenerateSubTree((root->right));

}

void GenerateExpression(struct bst_tok_node *root){
    printf("EXPRESSION GF@var\n");
}

void GenerateIF(struct bst_tok_node *root){
    GenerateExpression(root->left);
    printf("JUMPIFNEQ IFELSE GF@var bool@true\n");
    GenerateSubTree(root->right->left);
    GenerateSubTree(root->right->right);
}
