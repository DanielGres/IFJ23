#include "codegen.h"

int IfCounter = 0;
int DivJumpCounter = 0;
int WhileCounter = 0;
int FuncCounter = 0;
bool isDouble = false;

bst_node **god;

void Instructions() {

    printf(
        "LABEL readInt\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval int\n"
        "POPFRAME\n"
        "RETURN\n\n");

    printf(
        "LABEL readString\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval string\n"
        "POPFRAME\n"
        "RETURN\n\n");

    printf(
        "LABEL readFloat\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval float\n"
        "POPFRAME\n"
        "RETURN\n\n");

    printf(
        "LABEL print\n"
        "PUSHFRAME\n"
        "DEFVAR LF@writeval\n"
        "MOVE LF@writeval LF@%1\n"
        "WRITE LF@writeval\n"
        "POPFRAME\n"
        "RETURN\n\n");

    printf(
        "LABEL intToDouble\n"
        "PUSHFRAME\n"
        "DEFVAR LF@intval\n"
        "MOVE LF@intval LF@%1\n"
        "INT2FLOAT LF@retval LF@intval\n"
        "POPFRAME\n"
        "RETURN\n\n"
    );

    printf(
        "LABEL doubleToInt\n"
        "PUSHFRAME\n"
        "DEFVAR LF@intval\n"
        "MOVE LF@intval LF@%1\n"
        "FLOAT2INT LF@retval LF@intval\n"
        "POPFRAME\n"
        "RETURN\n\n\n"
    );

}

void GenerateSubTree(struct bst_tok_node *curr_root) {
    if (curr_root == NULL) return;
    switch (curr_root->T->dtype) {
        case letT: {
            GenerateLet(curr_root->left, false);
        } break;
        case varT: {
            GenerateVar(curr_root->left, false);
        } break;
        case ifT: {
            GenerateIF(curr_root->left, false);
        } break;
        case whileT: {
            GenerateWhile(curr_root->left, false);
        } break;
        case varidT: {
            CallFuncAssigment(curr_root, false);
        } break;
        case funcT: {
            GenerateFunctionDefinition(curr_root->left);
        } break;
        case returnT: {
            GenerateExpression(curr_root->left, false);
            printf("POPS LF@retval\n");
            printf("POPFRAME\n");
            printf("RETURN\n");
        } break;
        default: {
            return;
        }
    }
    GenerateSubTree(curr_root->right);
}

void GenerateSubTreeFunction(struct bst_tok_node *curr_root) {
    if (curr_root == NULL) return;
    switch (curr_root->T->dtype) {
        case letT: {
            GenerateLet(curr_root->left, true);
        } break;
        case varT: {
            GenerateVar(curr_root->left, true);
        } break;
        case ifT: {
            GenerateIF(curr_root->left, true);
        } break;
        case whileT: {
            GenerateWhile(curr_root->left, true);
        } break;
        case varidT: {
            CallFuncAssigment(curr_root, true);
        } break;
        default: {
            return;
        }
    }
    GenerateSubTree(curr_root->right);
}

void GenerateFunctionDefinition(struct bst_tok_node *root) {
    FuncCounter++;
    int count = FuncCounter;
    printf("JUMP ENDFUNC%d\n", count);
    printf("LABEL %s\n", root->T->val->s);
    printf("PUSHFRAME\n");
    if (root->right != NULL) {
        //printf("DEFVAR LF@retval\n");
        printf("MOVE LF@retval nil@nil\n");
    }
    // nacitanie parametrov
    PrepareFuncParams(root->left);
    // Body of function
    GenerateSubTreeFunction(root->left->right);
    // popripade return
    printf("POPFRAME\n");
    if (root->right != NULL){
        printf("PUSHS TF@retval\n");
    }
    printf("RETURN\n");
    printf("LABEL ENDFUNC%d\n", count);
}

void PrepareFuncParams(struct bst_tok_node *root) {
    root = root->left;
    int paramCnt = 1;
    while (root != NULL) {
        printf("DEFVAR LF@%s\n", root->right->T->val->s);
        printf("MOVE LF@%s LF@%%%d\n", root->right->T->val->s, paramCnt);
        root = root->left;
        paramCnt++;
    }
}

void CallFuncAssigment(struct bst_tok_node *root, bool inFunction) {
    if (root->left->T->dtype == LbracketT) {
        GenerateCallFunction(root);
    } else {
        GenerateAssigment(root, inFunction);
        if (Is_In_BTree(god, root->T->val->s)) {
            printf("POPS GF@%s\n", root->T->val->s);
        } else {
            printf("POPS LF@%s\n", root->T->val->s);
        }
    }
}

void PrepareFuncCallParams(struct bst_tok_node *root) {
    root = root->left;
    int paramCnt = 1;
    while (root != NULL) {
        printf("DEFVAR TF@%%%d\n", paramCnt);
        if(root->right != NULL){
            if(Is_In_BTree(god, root->right->T->val->s))
            {
                printf("MOVE TF@%%%d GF@%s\n", paramCnt, root->right->T->val->s);
            }
            else{
                if(root->right->T->dtype == doublenumT)
                {
                    printf("MOVE TF@%%%d float@%a\n", paramCnt, atof(root->right->T->val->s));
                }
                else if(root->right->T->dtype == intnumT)
                {
                    printf("MOVE TF@%%%d int@%s\n", paramCnt, root->right->T->val->s);
                }
                else if(root->right->T->dtype == stringT)
                {
                    printf("MOVE TF@%%%d string@%s\n", paramCnt, root->right->T->val->s);
                }
                else{
                    printf("MOVE TF@%%%d LF@%s\n", paramCnt, root->right->T->val->s);
                }

            }
        }
        else{
            if(Is_In_BTree(god, root->T->val->s))
            {
                printf("MOVE TF@%%%d GF@%s\n", paramCnt, root->T->val->s);
            }
            else{
                if(root->T->dtype == doublenumT)
                {
                    printf("MOVE TF@%%%d float@%a\n", paramCnt, atof(root->T->val->s));
                }
                else if(root->T->dtype == intnumT)
                {
                    printf("MOVE TF@%%%d int@%s\n", paramCnt, root->T->val->s);
                }
                else if(root->T->dtype == stringT)
                {
                    printf("MOVE TF@%%%d string@%s\n", paramCnt, root->T->val->s);
                }
                else{
                    printf("MOVE TF@%%%d LF@%s\n", paramCnt, root->T->val->s);
                }
            }
        }
        root = root->left;
        paramCnt++;
    }
}

// Function has passed two roots in order to return value to var
void GenerateCallFunction(struct bst_tok_node *root) {
    if (!strcmp(root->T->val->s, "write")) {
        GenerateCallWrite(root);
    } else if (!strcmp(root->T->val->s, "readInt")) {
        GenerateCallReadInt(root);
    } else if (!strcmp(root->T->val->s, "readString")) {
        GenerateCallReadString(root);
    } else if (!strcmp(root->T->val->s, "readDouble")) {
        GenerateCallReadFloat(root);
    } else if (!strcmp(root->T->val->s, "Int2Double")) {
        GenerateInt2Double(root->left);
    } else if (!strcmp(root->T->val->s, "Double2Int")) {
        GenerateDouble2Int(root->left);
    } else {
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@retval\n");
        PrepareFuncCallParams(root->left);
        printf("CALL %s\n",root->T->val->s);
        // printf("PUSHS TF@retval\n");
    };
}

void GenerateInt2Double(struct bst_tok_node *root) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@retval\n");
    printf("DEFVAR TF@%1\n");
    //printf("TOTO TU MAM HAHAHAHHA %s\n", root->left->T->val->s);
    if (root->left->T->dtype == varidT) {
        if(Is_In_BTree(god, root->left->T->val->s))
        {
            printf("MOVE TF@%1 GF@%s\n", root->left->T->val->s);
        }
        else{
            printf("MOVE TF@%1 LF@%s\n", root->left->T->val->s);
        }
        //printf("MOVE TF@%1 GF@%s\n", root->T->val->s);
    } else  {
        printf("MOVE TF@%1 int@%s\n", root->left->T->val->s);
    }
    printf("CALL intToDouble\n");
    printf("PUSHS TF@retval\n");
}

void GenerateDouble2Int(struct bst_tok_node *root) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@retval\n");
    printf("DEFVAR TF@%1\n");
    //printf("TOTO TU MAM HAHAHAHHA %s\n", root->left->T->val->s);
    if (root->left->T->dtype == varidT) {
        if(Is_In_BTree(god, root->left->T->val->s))
        {
            printf("MOVE TF@%1 GF@%s\n", root->left->T->val->s);
        }
        else{
            printf("MOVE TF@%1 LF@%s\n", root->left->T->val->s);
        }
        //printf("MOVE TF@%1 GF@%s\n", root->T->val->s);
    } else  {
        printf("MOVE TF@%1 int@%s\n", root->left->T->val->s);
    }
    printf("CALL doubleToInt\n");
    printf("PUSHS TF@retval\n");
}

void GenerateCallReadString(struct bst_tok_node *root) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@retval\n");
    printf("CALL readString\n");
    printf("PUSHS TF@retval\n");
}

void GenerateCallReadInt(struct bst_tok_node *root) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@retval\n");
    printf("CALL readInt\n");
    printf("PUSHS TF@retval\n");
}

void GenerateCallReadFloat(struct bst_tok_node *root) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@retval\n");
    printf("CALL readFloat\n");
    printf("PUSHS TF@retval\n");
}

void GenerateCallWrite(struct bst_tok_node *root) {
    root = root->left;
    while (root->left != NULL) {
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@%1\n");
        root = root->left;
        if (root->T->dtype == varidT) {
            if(Is_In_BTree(god, root->T->val->s))
            {
                printf("MOVE TF@%1 GF@%s\n", root->T->val->s);
            }
            else{
                printf("MOVE TF@%1 LF@%s\n", root->T->val->s);
            }
            //printf("MOVE TF@%1 GF@%s\n", root->T->val->s);
        } else if (root->T->dtype == intnumT) {
            printf("MOVE TF@%1 int@%s\n", root->T->val->s);
        } else if (root->T->dtype == doublenumT) {
            printf("MOVE TF@%1 float@%a\n", atof(root->T->val->s));
        } else if (root->T->dtype == stringT) {
            printf("MOVE TF@%1 string@%s\n", root->T->val->s);
        }

        printf("CALL print\n");
    }
}

void GenerateAssigment(struct bst_tok_node *root, bool inFunction) {
    GenerateExpression(root->left, inFunction);
}

void Generator(struct bst_tok_node *root, bst_node **kamisama) {
    god = (kamisama);
    //preorderTraversal(god);
    printf(".IFJcode23\n");
    printf("JUMP MAIN\n\n");
    Instructions();
    //PrintAllVariablesinScope(god, true);
    printf("LABEL MAIN\n");
    printf("DEFVAR GF@exp\n");
    printf("DEFVAR GF@floathelp1\n");
    printf("DEFVAR GF@floathelp2\n");
    printf("DEFVAR GF@typeresult\n");
    // Instructions();
    GenerateSubTree((root->right));
    printf("EXIT int@0\n");
}

void convertToFloatAndSwap(){
    DivJumpCounter++;
    printf("POPS GF@floathelp1\n");
    printf("TYPE GF@typeresult GF@floathelp1\n");
    // if second operand is float jump to check if second operand is float
    printf("JUMPIFEQ firstFloat%d GF@typeresult string@float\n", DivJumpCounter);
    printf("POPS GF@floathelp2\n");
    printf("TYPE GF@typeresult GF@floathelp2\n");
    // if second operand is float jump to convert first operand to float
    printf("JUMPIFEQ secondFloat%d GF@typeresult string@float\n", DivJumpCounter);
    printf("PUSHS GF@floathelp1\n");
    printf("PUSHS GF@floathelp2\n");
    printf("JUMP endDiv%d\n", DivJumpCounter);
    printf("LABEL firstFloat%d\n", DivJumpCounter);
    printf("POPS GF@floathelp2\n");
    printf("TYPE GF@typeresult GF@floathelp2\n");
    // if second operand is float jump to DIVS
    printf("JUMPIFEQ normalDiv%d GF@typeresult string@float\n", DivJumpCounter);
    printf("INT2FLOAT GF@floathelp2 GF@floathelp2\n");
    printf("JUMP normalDiv%d\n", DivJumpCounter);
    printf("LABEL secondFloat%d\n", DivJumpCounter);
    printf("INT2FLOAT GF@floathelp1 GF@floathelp1\n");
    printf("LABEL normalDiv%d\n", DivJumpCounter);
    printf("PUSHS GF@floathelp1\n");
    printf("PUSHS GF@floathelp2\n");
    printf("LABEL endDiv%d\n", DivJumpCounter);
}

void GenerateExprInstruction(struct bst_tok_node *root, bool inFunction) {
    switch (root->T->dtype) {
        case varidT: {
            //preorderTraversal(god);
            if (Is_In_BTree(god, root->T->val->s)) {
                printf("PUSHS GF@%s\n", root->T->val->s);
            } else {
                printf("PUSHS LF@%s\n", root->T->val->s);
            }
        } break;
        case intnumT: {
            printf("PUSHS int@%s\n", root->T->val->s);
        } break;
        case doublenumT: {
            //printf("HALO HALO PUSHS float@%s\n", root->T->val->s);
            printf("PUSHS float@%a\n", atof(root->T->val->s));

        } break;
        case stringT: {
            printf("PUSHS string@%s\n", root->T->val->s);
        } break;
        case operatorT: {
            if (!strcmp(root->T->val->s, "+")) {
                convertToFloatAndSwap();
                printf("ADDS\n");
            }
            if (!strcmp(root->T->val->s, "-")) {
                convertToFloatAndSwap();
                printf("SUBS\n");
            }
            if (!strcmp(root->T->val->s, "*")) {
                convertToFloatAndSwap();
                printf("MULS\n");
            }
            if (!strcmp(root->T->val->s, ".")) {
                printf("POPS GF@floathelp1\n");
                printf("POPS GF@floathelp2\n");
                printf("PUSHS GF@floathelp1\n");
                printf("PUSHS GF@floathelp2\n");
                printf("CONCATS\n");
            }
            if (!strcmp(root->T->val->s, "/")) {
                //printf("IDIVS\n");
                convertToFloatAndSwap();
                DivJumpCounter++;
                printf("POPS GF@floathelp1\n");
                printf("TYPE GF@typeresult GF@floathelp1\n");
                printf("JUMPIFEQ firstFloat%d GF@typeresult string@float\n", DivJumpCounter);
                printf("PUSHS GF@floathelp1\n");
                printf("IDIVS\n");
                printf("JUMP endDiv%d\n", DivJumpCounter);
                printf("LABEL firstFloat%d\n", DivJumpCounter);
                printf("PUSHS GF@floathelp1\n");
                printf("DIVS\n");
                printf("LABEL endDiv%d\n", DivJumpCounter);
            
            }
            if (!strcmp(root->T->val->s, "==")) {
                convertToFloatAndSwap();
                printf("EQS\n");
            }
            if (!strcmp(root->T->val->s, "!=")) {
                convertToFloatAndSwap();
                printf("EQS\n");
            }
            if (!strcmp(root->T->val->s, ">")) {
                convertToFloatAndSwap();
                printf("GTS\n");
            }
            if (!strcmp(root->T->val->s, "<")) {
                convertToFloatAndSwap();
                printf("LTS\n");
            }
        } break;
    }
}

void ExpressionPostorderTraversal(struct bst_tok_node *root, bool inFunction) {
    if (root == NULL) return;
    if (root->left != NULL && root->left->T->dtype == LbracketT) {
        GenerateCallFunction(root);
    } else {
        ExpressionPostorderTraversal(root->left, inFunction);
        ExpressionPostorderTraversal(root->right, inFunction);
        GenerateExprInstruction(root, inFunction);
    }
}

void GenerateExpression(struct bst_tok_node *root, bool inFunction) {
    ExpressionPostorderTraversal(root, inFunction);
}

void GenerateLet(struct bst_tok_node *root, bool inFunction) {
    if (Is_In_BTree(god, root->T->val->s)) {
        printf("DEFVAR GF@%s\n", root->T->val->s);
        GenerateExpression(root->left, inFunction);
        printf("POPS GF@exp\n");
        printf("MOVE GF@%s GF@exp\n", root->T->val->s);
    } else {
        printf("DEFVAR LF@%s\n", root->T->val->s);
        GenerateExpression(root->left, inFunction);
        printf("POPS GF@exp\n");
        printf("MOVE LF@%s GF@exp\n", root->T->val->s);
    }
    // Expression call
    // Result of expression will be on top of stack
}

void GenerateVar(struct bst_tok_node *root, bool inFunction) {
    if (Is_In_BTree(god, root->T->val->s)) {
        printf("DEFVAR GF@%s\n", root->T->val->s);
        GenerateExpression(root->left, inFunction);
        printf("POPS GF@exp\n");
        printf("MOVE GF@%s GF@exp\n", root->T->val->s);
    } else {
        printf("DEFVAR LF@%s\n", root->T->val->s);
        GenerateExpression(root->left, inFunction);
        printf("POPS GF@exp\n");
        printf("MOVE LF@%s GF@exp\n", root->T->val->s);
    }
}

void GenerateWhile(struct bst_tok_node *root, bool inFunction) {
    WhileCounter++;
    int thisWhile = WhileCounter;

    // While jump
    printf("LABEL WHILE%d\n", thisWhile);

    // While body
    GenerateSubTree(root->right->right);

    GenerateExpression(root->left, inFunction);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");
    // Skip while if not ture
    printf("JUMPIFEQ WHILE%d GF@exp bool@true\n", thisWhile);
}

void GenerateIF(struct bst_tok_node *root, bool inFunction) {
    IfCounter++;
    int thisIf = IfCounter;
    //printf("%s\n",root->left->T->val->s);
    if(root->left->T->dtype == letT){
        printf("JUMPIFNEQ IF%d nil@nil ",thisIf);
        Is_In_BTree(god, root->left->left->T->val->s)?printf("GF@"):printf("LF@");
        printf("%s\n",root->left->left->T->val->s);
    }
    else{
        GenerateExpression(root->left, inFunction);
        // Result of expression will be on top of stack
        printf("POPS GF@exp\n");

        // Jump IF ...
        printf("JUMPIFEQ IF%d GF@exp bool@true\n", thisIf);
    }

    // else part
    GenerateSubTree(root->right->right);
    // jump end
    printf("JUMP END%d\n", thisIf);
    // label if
    printf("LABEL IF%d\n", thisIf);
    GenerateSubTree(root->right->left);
    // label end
    printf("LABEL END%d\n", thisIf);
}
