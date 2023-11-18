// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#include "../include/expression.h"

token *myToken;
int condition = 0; // 1 if semicolon 2 if bracket

table_sign_enum prec_table[T_SIZE][T_SIZE] = {
  // 0  1  2  3  4  5  6  7  8
    {E, R, R, R, R, E, R, E, R}, // !
    {S, R, R, R, R, S, R, S, R}, // */
    {S, S, R, R, R, S, R, S, R}, // +-
    {S, S, S, R, R, S, R, S, R}, // == != <= >= < >
    {S, S, S, S, S, S, R, S, R}, // ??
    {S, S, S, S, S, S, N, S, E}, // (
    {E, R, R, R, R, E, R, E, R}, // )
    {R, R, R, R, R, E, R, E, R}, // id
    {S, S, S, S, S, S, E, S, E}  // $
};

table_symbol_enum get_table_symbol(token *loc_token, bst_node **root)
{
    if (dynstr_cmp(loc_token->val, "!")) 
    {
        return NOTNIL;
    }
    if (dynstr_cmp(loc_token->val, "+"))
    {
        return PLUS;
    }
    if (dynstr_cmp(loc_token->val, "-"))
    {
        return MINUS;
    }
    if (dynstr_cmp(loc_token->val, "*"))
    {
        return MUL;
    }
    if (dynstr_cmp(loc_token->val, "/"))
    {
        return DIV;
    }
    if (dynstr_cmp(loc_token->val, "=="))
    {
        return EQ;
    }
    if (dynstr_cmp(loc_token->val, "!="))
    {
        return NEQ;
    }
    if (dynstr_cmp(loc_token->val, "<="))
    {
        return LEQ;
    }
    if (dynstr_cmp(loc_token->val, ">="))
    {
        return GEQ;
    }
    if (dynstr_cmp(loc_token->val, "<"))
    {
        return LTN;
    }
    if (dynstr_cmp(loc_token->val, ">"))
    {
        return GTN;
    }
    if (dynstr_cmp(loc_token->val, "("))
    {
        return BRACKETS;
    }
    if (dynstr_cmp(loc_token->val, ")"))
    {
        return BRACKETE;
    }
    if (dynstr_cmp(loc_token->val, ";"))
    {
        return DOLLAR;
    }
    if ((loc_token->dtype == varidT) || (loc_token->dtype == vartypeT) || (loc_token->dtype == vartypeQT) || (loc_token->dtype == nilT))
    {
        if (loc_token->dtype == varidT)
        {
            Insert_BTree(root, myToken->val->s, myToken->dtype, false);
        }
        return IDENTIFIER;
    }
    else
    {
        exit(9);
        return false;
    }
}

int get_index_enum(table_symbol_enum symbolE)
{
    switch (symbolE)
    {
    case NOTNIL:
        return 0;
    case PLUS:
    case MINUS:
        return 1;
    case MUL:
    case DIV:
        return 2;
    case BRACKETS:
        return 3;
    case BRACKETE:
        return 4;
    case IDENTIFIER:
        return 5;
    case LEQ:
    case LTN:
    case GEQ:
    case GTN:
        return 6;
    case EQ:
    case NEQ:
        return 7;
    case DOLLAR:
        return 8;
    default:
        return;
        break;
    }
}

token *prevToken;

// NOT TESTED YET
bool expression(int *condition, bst_node **root, int isCallLater, struct bst_tok_node *seed)
{
    stack *my_stack = malloc(sizeof(stack));
    stack_init(my_stack);
    stack_push(my_stack, DOLLAR);

    stack_of_T *token_stack = malloc(sizeof(stack_of_T));
    stack_of_node *node_stack = malloc(sizeof(stack_of_node));
    stack_initT(token_stack);
    stack_init_node(node_stack);
    table_symbol_enum top;
    table_symbol_enum symbol;
    table_symbol_enum current_symbol;
    table_sign_enum table_symbol;
    table_symbol_enum reduc_symbol;

    top = stack_top(my_stack)->symbol;
    current_symbol = get_table_symbol(myToken, root);

    switch (isCallLater)
    {
    case 0:
        break;
    case 1:
        stack_push(my_stack, SHIFT);
        stack_push(my_stack, current_symbol);
        stack_pushT(token_stack, myToken);
        break;
    case 2:
        stack_pushT(token_stack, prevToken);
        stack_pushT(token_stack, myToken);
        stack_push(my_stack, ENTERPRISE);
        stack_push(my_stack, get_table_symbol(myToken, root));
        break;
    default:

        return false;
        break;
    }

    dyn_string buffer;
    dynstr_init(&buffer);

    if (!Get_Token(&myToken))
    {
        exit(1);
    }
    stack_pushT(token_stack, myToken);
    top = stack_top(my_stack)->symbol;
    current_symbol = get_table_symbol(myToken, root);

    while ((!((current_symbol == BRACKETE) || (current_symbol == DOLLAR))) || (!((my_stack->top->symbol == ENTERPRISE) && (item_symblcmp(my_stack->top->next, DOLLAR)))))
    {
        if (stack_top(my_stack)->symbol == ENTERPRISE)
        {
            stack_item *koks = ((stack_top(my_stack))->next);
            top = koks->symbol;
        }
        else
        {
            top = stack_top(my_stack)->symbol;
        }
        current_symbol = get_table_symbol(myToken, root);
        table_symbol = prec_table[get_index_enum(top)][get_index_enum(current_symbol)];

        switch (table_symbol)
        {
        case S:
            if (my_stack->top->symbol != ENTERPRISE)
            {
                stack_push(my_stack, SHIFT);
                stack_push(my_stack, current_symbol);
            }
            else
            {
                item_push(my_stack, my_stack->top->next, SHIFT);
                stack_push(my_stack, current_symbol);
            }

            if (!Get_Token(&myToken))
            {
                exit(1);
            }
            if (myToken->dtype != LbracketT && myToken->dtype != RbracketT && myToken->dtype != DOLLAR)
            {
                stack_pushT(token_stack, myToken);
            }
            break;
        case R:
            top = stack_top(my_stack)->symbol;
            while (top != SHIFT)
            {
                if (top == IDENTIFIER)
                {
                    dynstr_add(&buffer, 'i');
                }
                else if (top == NOTNIL) 
                {
                    dynstr_add(&buffer, '!');
                }
                else if (top == PLUS)
                {
                    dynstr_add(&buffer, '+');
                }
                else if (top == MINUS)
                {
                    dynstr_add(&buffer, '-');
                }
                else if (top == ENTERPRISE)
                {
                    dynstr_add(&buffer, 'E');
                }
                else if (top == MUL)
                {
                    dynstr_add(&buffer, '*');
                }
                else if (top == DIV)
                {
                    dynstr_add(&buffer, '/');
                }
                else if (top == EQ)
                {
                    dynstr_addstr(&buffer, "==");
                }
                else if (top == NEQ)
                {
                    dynstr_addstr(&buffer, "!=");
                }
                else if (top == LEQ)
                {
                    dynstr_addstr(&buffer, "<=");
                }
                else if (top == LTN)
                {
                    dynstr_addstr(&buffer, "<");
                }
                else if (top == GEQ)
                {
                    dynstr_addstr(&buffer, ">=");
                }
                else if (top == GTN)
                {
                    dynstr_addstr(&buffer, ">");
                }
                else if (top == BRACKETS)
                {
                    dynstr_add(&buffer, '(');
                }
                else if (top == BRACKETE)
                {
                    dynstr_add(&buffer, ')');
                }
                else
                {
                    exit(69);
                }

                stack_pop(my_stack);
                top = stack_top(my_stack)->symbol;
            }
            stack_pop(my_stack);

            if (dynstr_cmp(&buffer, "i"))
            {
                stack_item_of_T *tmpItem;

                if ((current_symbol == BRACKETE))
                {
                    tmpItem = &token_stack->top;
                }
                else
                {
                    tmpItem = getItemBelowTop(token_stack); // used to be prevKaffee()
                }

                struct bst_tok_node *tmpNode = Set_TokNode(&tmpItem->stackToken);
                stack_push_node(node_stack, tmpNode);
                stack_push(my_stack, ENTERPRISE);
            }
            else if (dynstr_cmp(&buffer, "E+E"))  {E_body("+", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E-E"))  {E_body("-", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E*E"))  {E_body("*", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E/E"))  {E_body("/", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, ")E("))  {stack_push(my_stack, ENTERPRISE);}
            else if (dynstr_cmp(&buffer, "E==E")) {E_body("==", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E!=E")) {E_body("!=", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E<E"))  {E_body("<", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E<=E")) {E_body("<=", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E>E"))  {E_body(">", my_stack, node_stack);}
            else if (dynstr_cmp(&buffer, "E>=E")) {E_body(">=", my_stack, node_stack);}
            // TODO pridat ?? a !
            else {exit(99);}
            dynstr_clear(&buffer);
            break;
        case N:
            stack_push(my_stack, current_symbol);
            if (!Get_Token(&myToken))
            {
                exit(1);
            }
            if ((myToken->dtype != LbracketT && myToken->dtype != RbracketT && myToken->dtype != DOLLAR))
            {
                stack_pushT(token_stack, myToken);
            }
            break;
        case E:
            return false;
            break;
        default:
            return false;
        }
    }
    if (current_symbol == DOLLAR)
    {
        *condition = 1;
    }
    if (current_symbol == BRACKETE)
    {
        *condition = 2;
    }
    stack_item_of_node *helper = node_stack->top->next;

    copySRCtoDST(seed, &(node_stack->top->ptr_to_node)); // used to be Ja()
    stack_free_node(node_stack);
    return true;
}

void E_body(const char* operator, stack* my_stack, stack_of_node* node_stack) {
    struct bst_tok_node* tmpNode1 = node_stack->top->ptr_to_node;
    stack_pop_node(node_stack);
    struct bst_tok_node* tmpNode2 = node_stack->top->ptr_to_node;
    stack_pop_node(node_stack);

    dyn_string string;
    dynstr_init(&string);
    dynstr_addstr(&string, operator);

    token* T;
    Token_init(&T);
    Token_set(&T, &string, operatorT);

    struct bst_tok_node* tmpNode3 = Create_TokTree(T, tmpNode2, tmpNode1);

    stack_push_node(node_stack, tmpNode3);
    stack_push(my_stack, ENTERPRISE);
}