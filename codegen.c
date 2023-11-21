#include "codegen.h"

int IfCounter = 0;

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
    int thisIf = IfCounter;
    GenerateExpression(root->left);
    // Jump IF ...
    printf("JUMPIFNEQ IF%d GF@var bool@true\n",thisIf);
    // else part
    GenerateSubTree(root->right->right);
    // jump end
    printf("JUMP END%d\n",thisIf);
    // label if
    printf("LABEL IF%d\n",thisIf);
    GenerateSubTree(root->right->left);
    // label end
    printf("LABEL END%d\n",thisIf);
}
