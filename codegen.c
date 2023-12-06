#include "codegen.h"

int IfCounter = 0;
int DivJumpCounter = 0;
int WhileCounter = 0;
int FuncCounter = 0;
int doubleMarker = 0;
bool isDouble = false;

bst_node **symbolGlob;

void Instructions() {
    // Built-in functions

    // Read Int function returns int trough retval
    printf(
        "LABEL readInt\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval int\n"
        "POPFRAME\n"
        "RETURN\n\n");

    // Read String function returns string trough retval
    printf(
        "LABEL readString\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval string\n"
        "POPFRAME\n"
        "RETURN\n\n");

    // Read Float function returns float trough retval
    printf(
        "LABEL readFloat\n"
        "CREATEFRAME\n"
        "PUSHFRAME\n"
        "DEFVAR LF@retval\n"
        "MOVE LF@retval nil@nil\n"
        "READ LF@retval float\n"
        "POPFRAME\n"
        "RETURN\n\n");

    // Print function prints value of variable passed
    printf(
        "LABEL print\n"
        "PUSHFRAME\n"
        "DEFVAR LF@writeval\n"
        "MOVE LF@writeval LF@%1\n"
        "WRITE LF@writeval\n"
        "POPFRAME\n"
        "RETURN\n\n");

    // Int to double function
    printf(
        "LABEL intToDouble\n"
        "PUSHFRAME\n"
        "DEFVAR LF@intval\n"
        "MOVE LF@intval LF@%1\n"
        "INT2FLOAT LF@retval LF@intval\n"
        "POPFRAME\n"
        "RETURN\n\n"
    );
    
    // Double to int function
    printf(
        "LABEL doubleToInt\n"
        "PUSHFRAME\n"
        "DEFVAR LF@intval\n"
        "MOVE LF@intval LF@%1\n"
        "FLOAT2INT LF@retval LF@intval\n"
        "POPFRAME\n"
        "RETURN\n\n"
    );

    // Length function uses STRLEN instruction to get length of string passed
    printf(
        "LABEL length\n"
        "PUSHFRAME\n"
        "DEFVAR LF@string\n"
        "MOVE LF@string LF@%1\n"
        "STRLEN LF@retval LF@string\n"
        "POPFRAME\n"
        "RETURN\n\n\n"
    );

}


void GenerateSubTree(struct bst_tok_node *curr_root) {
    // Function recursively rides along right side of the tree and generates code for each node until end of tree
    if (curr_root == NULL) return;

    // Switch for each type of node that can be found in right side of tree
    switch (curr_root->T->dtype) {
        case letT: {
            GenerateLet(curr_root->left);
        } break;
        case varT: {
            GenerateVar(curr_root->left);
        } break;
        case ifT: {
            GenerateIF(curr_root->left);
        } break;
        case whileT: {
            GenerateWhile(curr_root->left);
        } break;
        case varidT: {
            CallFuncAssigment(curr_root);
        } break;
        case funcT: {
            GenerateFunctionDefinition(curr_root->left);
        } break;
        default: {
            return;
        }
    }
    // Recursive call for right side of tree
    GenerateSubTree(curr_root->right);
}

void GenerateSubTreeFunction(struct bst_tok_node *curr_root) {
    // Function recursively rides along right side of the tree and generates code for each node until end of tree inside a function
    if (curr_root == NULL) return;
    switch (curr_root->T->dtype) {
        case letT: {
            GenerateLet(curr_root->left);
        } break;
        case varT: {
            GenerateVar(curr_root->left);
        } break;
        case ifT: {
            GenerateIF(curr_root->left);
        } break;
        case whileT: {
            GenerateWhile(curr_root->left);
        } break;
        case varidT: {
            CallFuncAssigment(curr_root);
        } break;
        // Return expression if function has return type
        case returnT: {
            if(curr_root->left != NULL){
                GenerateExpression(curr_root->left);
                printf("POPS LF@retval\n");

            }
            printf("POPFRAME\n");
            printf("RETURN\n");
        } break;
        default: {
            return;
        }
    }
    // Recursive call for right side of tree
    GenerateSubTree(curr_root->right);
}

void GenerateFunctionDefinition(struct bst_tok_node *root) {
    // Counter to generate unique labels for each function to skip function body if not called
    FuncCounter++;
    int count = FuncCounter;
    printf("JUMP ENDFUNC%d\n", count);
    // Label for function, program will jump here if function was called
    printf("LABEL %s\n", root->T->val->s);
    printf("PUSHFRAME\n");
    if (root->right != NULL) {
        printf("MOVE LF@retval nil@nil\n");
    }

    // Prepares parameter that are passed to function
    PrepareFuncParams(root->left);
    // Body of function
    GenerateSubTreeFunction(root->left->right);
    // End of function
    printf("POPFRAME\n");
    if (root->right != NULL){
        printf("PUSHS TF@retval\n");
    }
    printf("RETURN\n");
    printf("LABEL ENDFUNC%d\n", count);
}

void PrepareFuncParams(struct bst_tok_node *root) {
    // Iteratively load parameters
    root = root->left;
    int paramCnt = 1;
    while (root != NULL) {
        printf("DEFVAR LF@%s\n", root->right->T->val->s);
        printf("MOVE LF@%s LF@%%%d\n", root->right->T->val->s, paramCnt);
        root = root->left;
        paramCnt++;
    }
}

void CallFuncAssigment(struct bst_tok_node *root) {
    if (root->left->T->dtype == LbracketT) {
        GenerateCallFunction(root);
    } else {
        GenerateAssigment(root);
        // Add result into variable
        if (Is_In_BTree(symbolGlob, root->T->val->s)) {
            printf("POPS GF@%s\n", root->T->val->s);
        } else {
            printf("POPS LF@%s\n", root->T->val->s);
        }
    }
}

void PrepareFuncCallParams(struct bst_tok_node *root) {
    // Iterative dive into all parameters
    root = root->left;
    int paramCnt = 1;
    while (root != NULL) {
        printf("DEFVAR TF@%%%d\n", paramCnt);
        if(root->right != NULL){
            if(Is_In_BTree(symbolGlob, root->right->T->val->s))
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
            if(Is_In_BTree(symbolGlob, root->T->val->s))
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
    }else if (!strcmp(root->T->val->s, "length")) {
        GenerateLength(root->left);
    } else {
        // Programmer defined function
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@retval\n");
        PrepareFuncCallParams(root->left);
        printf("CALL %s\n",root->T->val->s);
        printf("PUSHS TF@retval\n");
    };
}

void GenerateLength(struct bst_tok_node *root) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@retval\n");
    printf("DEFVAR TF@%1\n");
    // Correctly passes parameter
    if (root->left->T->dtype == varidT) {
        if(Is_In_BTree(symbolGlob, root->left->T->val->s))
        {
            printf("MOVE TF@%1 GF@%s\n", root->left->T->val->s);
        }
        else{
            printf("MOVE TF@%1 LF@%s\n", root->left->T->val->s);
        }
    } else  {
        printf("MOVE TF@%1 string@%s\n", root->left->T->val->s);
    }
    printf("CALL length\n");
    printf("PUSHS TF@retval\n");
}

void GenerateInt2Double(struct bst_tok_node *root) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@retval\n");
    printf("DEFVAR TF@%1\n");

    if (root->left->T->dtype == varidT) {
        if(Is_In_BTree(symbolGlob, root->left->T->val->s))
        {
            printf("MOVE TF@%1 GF@%s\n", root->left->T->val->s);
        }
        else{
            printf("MOVE TF@%1 LF@%s\n", root->left->T->val->s);
        }
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

    if (root->left->T->dtype == varidT) {
        if(Is_In_BTree(symbolGlob, root->left->T->val->s))
        {
            printf("MOVE TF@%1 GF@%s\n", root->left->T->val->s);
        }
        else{
            printf("MOVE TF@%1 LF@%s\n", root->left->T->val->s);
        }
    } else  {
        printf("MOVE TF@%1 float@%a\n", atof(root->left->T->val->s));
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
    // Iterative call for write of multiple parameters
    root = root->left;
    while (root->left != NULL) {
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@%1\n");
        root = root->left;
        if (root->T->dtype == varidT) {
            if(Is_In_BTree(symbolGlob, root->T->val->s))
            {
                printf("MOVE TF@%1 GF@%s\n", root->T->val->s);
            }
            else{
                printf("MOVE TF@%1 LF@%s\n", root->T->val->s);
            }
        } else if (root->T->dtype == intnumT) {
            printf("MOVE TF@%1 int@%s\n", root->T->val->s);
        } else if (root->T->dtype == doublenumT) {
            printf("MOVE TF@%1 float@%a\n", atof(root->T->val->s));
        } else if (root->T->dtype == stringT) {
            printf("MOVE TF@%1 string@%s\n", root->T->val->s);
        } else if (root->T->dtype == nilT) {
            printf("MOVE TF@%1 nil@nil\n");
        }

        printf("CALL print\n");
    }
}

void GenerateAssigment(struct bst_tok_node *root) {
    GenerateExpression(root->left);
}

void Generator(struct bst_tok_node *root, bst_node **tableOfSymbols) {
    symbolGlob = (tableOfSymbols);

    printf(".IFJcode23\n");
    printf("JUMP MAIN\n\n");
    Instructions();

    printf("LABEL MAIN\n");
    printf("DEFVAR GF@exp\n");
    printf("DEFVAR GF@equal1\n");
    printf("DEFVAR GF@equal2\n");
    printf("DEFVAR GF@floathelp1\n");
    printf("DEFVAR GF@floathelp2\n");
    printf("DEFVAR GF@typeresult\n");
    
    // Generate main code
    GenerateSubTree((root->right));
    printf("EXIT int@0\n");
}

void convertToFloatAndSwap(){
    DivJumpCounter++;
    printf("POPS GF@floathelp1\n");
    printf("TYPE GF@typeresult GF@floathelp1\n");
    // If second operand is float jump to check if second operand is float
    printf("JUMPIFEQ firstFloat%d GF@typeresult string@float\n", DivJumpCounter);
    printf("POPS GF@floathelp2\n");
    printf("TYPE GF@typeresult GF@floathelp2\n");
    // If second operand is float jump to convert first operand to float
    printf("JUMPIFEQ secondFloat%d GF@typeresult string@float\n", DivJumpCounter);
    printf("PUSHS GF@floathelp1\n");
    printf("PUSHS GF@floathelp2\n");
    printf("JUMP endDiv%d\n", DivJumpCounter);
    printf("LABEL firstFloat%d\n", DivJumpCounter);
    printf("POPS GF@floathelp2\n");
    printf("TYPE GF@typeresult GF@floathelp2\n");
    // If second operand is float jump to DIVS
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

void checkString(){
    DivJumpCounter++;
    printf("POPS GF@floathelp1\n");
    printf("TYPE GF@typeresult GF@floathelp1\n");
    printf("PUSHS GF@floathelp1\n");
    printf("JUMPIFNEQ string%d GF@typeresult string@string\n", DivJumpCounter);
    printf("POPS GF@floathelp1\n");
    printf("POPS GF@floathelp2\n");
    printf("CONCAT GF@floathelp2 GF@floathelp2 GF@floathelp1\n");
    printf("PUSHS GF@floathelp2\n");
    printf("JUMP endString%d\n", DivJumpCounter);
    printf("LABEL string%d\n",DivJumpCounter);
    printf("ADDS\n");
    printf("LABEL endString%d\n",DivJumpCounter);
}

void GenerateExprInstruction(struct bst_tok_node *root) {
    switch (root->T->dtype) {
        // Pushing variables to stack
        case varidT: {
            if (Is_In_BTree(symbolGlob, root->T->val->s)) {
                printf("PUSHS GF@%s\n", root->T->val->s);
            } else {
                printf("PUSHS LF@%s\n", root->T->val->s);
            }
        } break;
        // Pushing constants to stack
        case intnumT: {
            printf("PUSHS int@%s\n", root->T->val->s);
        } break;
        case doublenumT: {
            printf("PUSHS float@%a\n", atof(root->T->val->s));

        } break;
        case stringT: {
            printf("PUSHS string@%s\n", root->T->val->s);
        } break;
        case nilT: {
            printf("PUSHS nil@nil\n");
        } break;
        // Arithmetic operations
        case operatorT: {
            if (!strcmp(root->T->val->s, "+")) {
                convertToFloatAndSwap();
                checkString();
            }
            if (!strcmp(root->T->val->s, "-")) {
                convertToFloatAndSwap();
                printf("SUBS\n");
            }
            if (!strcmp(root->T->val->s, "*")) {
                convertToFloatAndSwap();
                printf("MULS\n");
            }
            if (!strcmp(root->T->val->s, "/")) {
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
                printf("NOTS\n");
            }
            if (!strcmp(root->T->val->s, ">")) {
                convertToFloatAndSwap();
                printf("GTS\n");
            }
            if (!strcmp(root->T->val->s, "<")) {
                convertToFloatAndSwap();
                printf("LTS\n");
            }
            if (!strcmp(root->T->val->s, ">=")) {
                convertToFloatAndSwap();
                printf("POPS GF@equal1\n");
                printf("POPS GF@equal2\n");
                printf("PUSHS GF@equal2\n");
                printf("PUSHS GF@equal1\n");
                printf("GTS\n");
                printf("PUSHS GF@equal2\n");
                printf("PUSHS GF@equal1\n");
                printf("EQS\n");
                printf("ORS\n");
            }
            if (!strcmp(root->T->val->s, "<=")) {
                convertToFloatAndSwap();
                printf("POPS GF@equal1\n");
                printf("POPS GF@equal2\n");
                printf("PUSHS GF@equal2\n");
                printf("PUSHS GF@equal1\n");
                printf("LTS\n");
                printf("PUSHS GF@equal2\n");
                printf("PUSHS GF@equal1\n");
                printf("EQS\n");
                printf("ORS\n");
            }
            if(!strcmp(root->T->val->s, "??")){
                convertToFloatAndSwap();
                printf("POPS GF@floathelp1\n");
                printf("POPS GF@floathelp2\n");
                doubleMarker++;
                // Check if type of left operand is nil
                printf("TYPE GF@typeresult GF@floathelp2\n");
                printf("JUMPIFEQ doubleMarker%d string@nil GF@typeresult\n",doubleMarker);
                printf("PUSHS GF@floathelp2\n");
                printf("JUMP endDoubleMarker%d\n",doubleMarker);
                printf("LABEL doubleMarker%d\n",doubleMarker);
                printf("PUSHS GF@floathelp1\n");
                printf("LABEL endDoubleMarker%d\n",doubleMarker);

            }
        } break;
    }
}

void ExpressionPostorderTraversal(struct bst_tok_node *root) {
    if (root == NULL) return;
    if (root->left != NULL && root->left->T->dtype == LbracketT) {
        GenerateCallFunction(root);
    } else {
        // Postorder traversal
        ExpressionPostorderTraversal(root->left);
        ExpressionPostorderTraversal(root->right);
        GenerateExprInstruction(root);
    }
}

void GenerateExpression(struct bst_tok_node *root) {
    ExpressionPostorderTraversal(root);
}

void GenerateLet(struct bst_tok_node *root) {
    // Decides if variable is global or local
    if (Is_In_BTree(symbolGlob, root->T->val->s)) {
        printf("DEFVAR GF@%s\n", root->T->val->s);
        if(root->left != NULL){
            GenerateExpression(root->left);
            printf("POPS GF@exp\n");
            printf("MOVE GF@%s GF@exp\n", root->T->val->s);
        }
        else{
            printf("MOVE GF@%s nil@nil\n", root->T->val->s);
        }
    } else {
        printf("DEFVAR LF@%s\n", root->T->val->s);
        if(root->left != NULL){
            GenerateExpression(root->left);
            printf("POPS GF@exp\n");
            printf("MOVE LF@%s GF@exp\n", root->T->val->s);
        }
        else{
            printf("MOVE LF@%s nil@nil\n", root->T->val->s);
        }
    }
}

void GenerateVar(struct bst_tok_node *root) {
    // Decides if variable is global or local
    if (Is_In_BTree(symbolGlob, root->T->val->s)) {
        printf("DEFVAR GF@%s\n", root->T->val->s);
        if(root->left != NULL){
            GenerateExpression(root->left);
            printf("POPS GF@exp\n");
            printf("MOVE GF@%s GF@exp\n", root->T->val->s);
        }
        else{
            printf("MOVE GF@%s nil@nil\n", root->T->val->s);
        }
    } else {
        printf("DEFVAR LF@%s\n", root->T->val->s);
        if(root->left != NULL){
            GenerateExpression(root->left);
            printf("POPS GF@exp\n");
            printf("MOVE LF@%s GF@exp\n", root->T->val->s);
        }
        else{
            printf("MOVE LF@%s nil@nil\n", root->T->val->s);
        }
    }
}

void GenerateWhile(struct bst_tok_node *root) {
    // Unique label for each while
    WhileCounter++;
    int thisWhile = WhileCounter;

    // While jump
    printf("LABEL WHILE%d\n", thisWhile);

    // While body
    GenerateSubTree(root->right->right);

    // While condition
    GenerateExpression(root->left);
    // Result of expression will be on top of stack
    printf("POPS GF@exp\n");
    // Skip while if not ture
    printf("JUMPIFEQ WHILE%d GF@exp bool@true\n", thisWhile);
}

void GenerateIF(struct bst_tok_node *root) {
    // Unique label for each if
    IfCounter++;
    int thisIf = IfCounter;

    // Special case for if not containing expression but inicialization of variable
    if(root->left->T->dtype == letT){
        printf("JUMPIFNEQ IF%d nil@nil ",thisIf);
        Is_In_BTree(symbolGlob, root->left->left->T->val->s)?printf("GF@"):printf("LF@");
        printf("%s\n",root->left->left->T->val->s);
    }
    else{
        GenerateExpression(root->left);
        // Result of expression will be on top of stack
        printf("POPS GF@exp\n");

        // Jump IF ...
        printf("JUMPIFEQ IF%d GF@exp bool@true\n", thisIf);
    }

    // Else part
    GenerateSubTree(root->right->right);
    // Jump end
    printf("JUMP END%d\n", thisIf);
    // Label if
    printf("LABEL IF%d\n", thisIf);
    GenerateSubTree(root->right->left);
    // Label end
    printf("LABEL END%d\n", thisIf);
}
