#include "../include/codegen.h"

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
    //printf("%d ",curr_root->T->dtype);
    switch(curr_root->T->dtype){
        case letT:{
            GenerateLet(curr_root->left);
        }
        break;
        case varT:{
            printf("DEFVAR GF@myvar\n");
            printf("MOVE GF@myvar somevar\n");
        }
        break;
        case ifT:{
            GenerateIF(curr_root->left);
        }
        break;
        case varidT:{
            GenerateAssigment(curr_root);
        }
        break;
        default:{
            exit (0);
        }
    }
    GenerateSubTree(curr_root->right);
}

void GenerateAssigment(struct bst_tok_node *root){
    printf("MOVE GF@%s int@%s\n", root->T->val->s, root->left->left->T->val->s);
}

void Generator(struct bst_tok_node *root){
    printf(".IFJcode23\n");
    printf("DEFVAR GF@exp\n");
    //Instructions();
    GenerateSubTree((root->right));
    printf("EXIT int@0\n");
}

void GenerateExpStack(struct bst_tok_node *root){
    if(root == NULL) return;
    printf("PUSHS %s\n",root->T->val->s);}

void ExpressionPostorderTraversal(struct bst_tok_node *root){
    if(root == NULL) return;
    ExpressionPostorderTraversal(root->left);
    ExpressionPostorderTraversal(root->right);
    GenerateExpression(root);
}

void GenerateExpression(struct bst_tok_node *root){
    ExpressionPostorderTraversal(root->left);
}

void GenerateLet(struct bst_tok_node *root){
    printf("DEFVAR GF@%s\n", root->T->val->s);
    if(root->left != NULL){
        printf("MOVE GF@%s int@%s\n", root->T->val->s, root->left->T->val->s);
    }
}

void GenerateIF(struct bst_tok_node *root){
    IfCounter++;
    int thisIf = IfCounter;
    //GenerateExpression(root->left);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");

    // Jump IF ...
    printf("JUMPIFNEQ IF%d GF@exp bool@true\n",thisIf);
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
