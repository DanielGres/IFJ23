#include "../include/codegen.h"

int IfCounter = 0;
int WhileCounter = 0;
int WhileCounter = 0;

void Instructions(){
    // printf(
    //     "LABEL READSTRING\n"
    //     "CREATEFRAME\n"
    //     "PUSHFRAME\n"
    //     "DEFVAR LF@retval\n"
    //     "MOVE LF@retval nil@nil\n"
    //     "READ LF@retval string\n"
    //     "POPFRAME\n"
    //     "RETURN\n"
    //     );
    
    // printf(
    //     "LABEL READINT\n"
    //     "CREATEFRAME\n"
    //     "PUSHFRAME\n"
    //     "DEFVAR LF@retval\n"
    //     "MOVE LF@retval nil@nil\n"
    //     "READ LF@retval int\n"
    //     "POPFRAME\n"
    //     "RETURN\n"
    //     );

    // printf(
    //     "LABEL READDOUBLE\n"
    //     "CREATEFRAME\n"
    //     "PUSHFRAME\n"
    //     "DEFVAR LF@retval\n"
    //     "MOVE LF@retval nil@nil\n"
    //     "READ LF@retval double\n"
    //     "POPFRAME\n"
    //     "RETURN\n"
    //     );

    // printf(
    //     "LABEL READBOOL\n"
    //     "CREATEFRAME\n"
    //     "PUSHFRAME\n"
    //     "DEFVAR LF@retval\n"
    //     "MOVE LF@retval nil@nil\n"
    //     "READ LF@retval bool\n"
    //     "POPFRAME\n"
    //     "RETURN\n"
    //     );
    
    printf(
        "LABEL print\n"
        "PUSHFRAME\n"
        "DEFVAR LF@writeval\n"
        "MOVE LF@writeval LF@%1\n"
        "WRITE LF@writeval\n"
        "POPFRAME\n"
        "RETURN\n\n"
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
            GenerateVar(curr_root->left);
        }
        break;
        case ifT:{
            GenerateIF(curr_root->left);
        }
        break;
        case whileT:{
            GenerateWhile(curr_root->left);
        }
        break;
        case varidT:{
            CallFuncAssigment(curr_root);
        }
        break;
        default:{
            exit (0);
        }
    }
    GenerateSubTree(curr_root->right);
}

void CallFuncAssigment(struct bst_tok_node *root){
    if(root->left->T->dtype == LbracketT){
        GenerateCallFunction(root);
    }
    else{
        GenerateAssigment(root);
    }
}
void GenerateCallFunction(struct bst_tok_node *root){
    if (!strcmp(root->T->val->s, "write")){
        root = root->left;
        while(root->left != NULL){
            printf("CREATEFRAME\n");
            printf("DEFVAR TF@%1\n");
            root = root->left;
            if(root->T->dtype == varidT){
                printf("MOVE TF@%1 GF@%s\n", root->T->val->s);
            }
            else if(root->T->dtype == intnumT){
                printf("MOVE TF@%1 int@%s\n", root->T->val->s);
            }
            else if(root->T->dtype == doublenumT){
                printf("MOVE TF@%1 float@%s\n", root->T->val->s);
            }
            else if(root->T->dtype == stringT){
                printf("MOVE TF@%1 string@%s\n", root->T->val->s);
            }
            
            printf("CALL print\n");
        }
    }
}

void GenerateAssigment(struct bst_tok_node *root){
    GenerateExpression(root->left);
    printf("POPS GF@%s\n", root->T->val->s);
}

void Generator(struct bst_tok_node *root){
    printf(".IFJcode23\n");
    printf("JUMP MAIN\n\n");
    Instructions();
    printf("LABEL MAIN\n");
    printf("DEFVAR GF@exp\n");
    //Instructions();
    GenerateSubTree((root->right));
    printf("EXIT int@0\n");
}

void GenerateExprInstruction(struct bst_tok_node *root){
    switch(root->T->dtype){
        case varidT:{
            printf("PUSHS GF@%s\n", root->T->val->s);
        }
        break;
        case intnumT:{
            printf("PUSHS int@%s\n", root->T->val->s);
        }
        break;
        case operatorT:{
            if (!strcmp(root->T->val->s, "+"))
            {
                printf("ADDS\n");
            }
            if (!strcmp(root->T->val->s, "-"))
            {
                printf("SUBS\n");
            }
            if (!strcmp(root->T->val->s, "*"))
            {
                printf("MULS\n");
            }
            if (!strcmp(root->T->val->s, "."))
            {
                printf("CONCATS\n");
            }
            if (!strcmp(root->T->val->s, "/"))
            {
                printf("DIVS\n");
            }
            if (!strcmp(root->T->val->s, "=="))
            {
                printf("EQS\n");
            }
            if (!strcmp(root->T->val->s, "!="))
            {
                printf("EQS\n");
            }
        }
        break;
    }
}

void ExpressionPostorderTraversal(struct bst_tok_node *root){
    if(root == NULL) return;
    ExpressionPostorderTraversal(root->left);
    ExpressionPostorderTraversal(root->right);
    GenerateExprInstruction(root);
}

void GenerateExpression(struct bst_tok_node *root){
    ExpressionPostorderTraversal(root);
}

void GenerateLet(struct bst_tok_node *root){
    printf("DEFVAR GF@%s\n", root->T->val->s);
    // Expression call
    GenerateExpression(root->left);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");
    printf("MOVE GF@%s GF@exp\n", root->T->val->s);
}

void GenerateVar(struct bst_tok_node *root){
    printf("DEFVAR GF@%s\n", root->T->val->s);
    // Expression call
    GenerateExpression(root->left);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");
    printf("MOVE GF@%s GF@exp\n", root->T->val->s);
}

void GenerateWhile(struct bst_tok_node *root){
    WhileCounter++;
    int thisWhile = WhileCounter;
    
    // While jump
    printf("LABEL WHILE%d\n",thisWhile);
    
    // While body
    GenerateSubTree(root->right->right);

    GenerateExpression(root->left);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");
    // Skip while if not ture
    printf("JUMPIFEQ WHILE%d GF@exp bool@false\n",thisWhile);

}

void GenerateIF(struct bst_tok_node *root){
    IfCounter++;
    int thisIf = IfCounter;
    GenerateExpression(root->left);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");

    // Jump IF ...
    printf("JUMPIFEQ IF%d GF@exp bool@true\n",thisIf);
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

