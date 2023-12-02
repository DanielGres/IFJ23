#include "codegen.h"

int IfCounter = 0;
int DivJumpCounter = 0;
int WhileCounter = 0;
int FuncCounter = 0;
bool isDouble = false;

bst_node **god;

void Instructions() {
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

    printf(
        "LABEL readInt\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval int\n"
        "POPFRAME\n"
        "RETURN\n");

    printf(
        "LABEL readString\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval string\n"
        "POPFRAME\n"
        "RETURN\n");

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
        "RETURN\n\n");
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
        printf("DEFVAR LF@retval");
        printf("MOVE LF@retval nil@nil\n");
    }
    // nacitanie parametrov
    PrepareFuncParams(root->left);
    // body
    GenerateSubTreeFunction(root->left->right);
    // popripade return
    printf("LABEL ENDFUNC%d\n", count);
}

void PrepareFuncParams(struct bst_tok_node *root) {
    root = root->left;
    int paramCnt = 1;
    while (root != NULL) {
        printf("DEFVAR LF@%s\n", root->right->T->val->s);
        printf("MOVE LF@%s TF@%%%d\n", root->right->T->val->s, paramCnt);
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

// Function has passed two roots in order to return value to var
void GenerateCallFunction(struct bst_tok_node *root) {
    if (!strcmp(root->T->val->s, "write")) {
        GenerateCallWrite(root);
    } else if (!strcmp(root->T->val->s, "readInt")) {
        GenerateCallReadInt(root);
    } else if (!strcmp(root->T->val->s, "readString")) {
        GenerateCallReadString(root);
    } else {
        printf("CALL %s\n",root->T->val->s);
    };
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

void GenerateCallWrite(struct bst_tok_node *root) {
    root = root->left;
    while (root->left != NULL) {
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@%1\n");
        root = root->left;
        if (root->T->dtype == varidT) {
            printf("MOVE TF@%1 GF@%s\n", root->T->val->s);
        } else if (root->T->dtype == intnumT) {
            printf("MOVE TF@%1 int@%s\n", root->T->val->s);
        } else if (root->T->dtype == doublenumT) {
            printf("MOVE TF@%1 float@%s\n", root->T->val->s);
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
        printf("POPS LF@exp\n");
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
    printf("JUMPIFEQ WHILE%d GF@exp bool@false\n", thisWhile);
}

void GenerateIF(struct bst_tok_node *root, bool inFunction) {
    IfCounter++;
    int thisIf = IfCounter;
    GenerateExpression(root->left, inFunction);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");

    // Jump IF ...
    printf("JUMPIFEQ IF%d GF@exp bool@true\n", thisIf);
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
