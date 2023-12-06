#include "expression.h"

#include "syntax.h"

// I LOVE BURRITO

extern token *myToken;
extern bool TakeToken;
#define GetToken()           \
    if (TakeToken) {         \
        Get_Token(&myToken); \
    } else {                 \
        TakeToken = true;    \
    }

table_sign_enum prec_table[T_SIZE][T_SIZE] = {
    // 0  1  2  3  4  5  6  7  8  9 10 11
    {E, R, R, R, R, R, R, R, R, E, E, R},  // !
    {S, R, R, R, R, R, S, R, S, S, S, R},  // */
    {S, S, R, R, R, R, S, R, S, S, S, R},  // +-
    {S, S, S, R, R, R, S, R, S, S, S, R},  // == != <= >= < >
    {S, S, S, R, R, R, S, R, S, S, S, R},  // && ||
    {S, S, S, S, S, S, S, R, S, S, S, R},  // ??
    {S, S, S, S, S, S, S, N, S, S, S, E},  // (
    {E, R, R, R, R, R, E, R, R, E, E, R},  // )
    {S, E, E, S, S, R, E, R, E, E, E, R},  // bool variable
    {E, R, R, R, R, R, E, R, E, E, E, R},  // value
    {R, R, R, R, R, R, F, R, E, E, E, R},  // id
    {S, S, S, S, S, S, S, E, S, S, S, E}   // $
};

int get_index_enum(table_symbol_enum symbolE) {
    switch (symbolE) {
        case NOT_NOTNIL:
            return 0;
        case MUL:
        case DIV:
            return 1;
        case PLUS:
        case MINUS:
            return 2;
        case EQ:
        case NEQ:
        case LEQ:
        case LTN:
        case GEQ:
        case GTN:
            return 3;
        case LAND:
        case LOR:
            return 4;
        case DBLQ:
            return 5;
        case BRACKETS:
            return 6;
        case BRACKETE:
            return 7;
        case BOOLVAR:
            return 8;
        case VALUE:
            return 9;
        case IDENTIFIER:
            return 10;
        case DOLLAR:
            return 11;
        default:
            return 33;
            break;
    }
}

table_symbol_enum get_table_symbol(token *loc_token, bst_node **root) {
    // DEL printf("Token :%d\n", loc_token->dtype);
    if (dynstr_cmp(loc_token->val, "!")) {
        return NOT_NOTNIL;
    } else if (dynstr_cmp(loc_token->val, "*")) {
        return MUL;
    } else if (dynstr_cmp(loc_token->val, "/")) {
        return DIV;
    } else if (dynstr_cmp(loc_token->val, "+")) {
        return PLUS;
    } else if (dynstr_cmp(loc_token->val, "-")) {
        return MINUS;
    } else if (dynstr_cmp(loc_token->val, "==")) {
        return EQ;
    } else if (dynstr_cmp(loc_token->val, "!=")) {
        return NEQ;
    } else if (dynstr_cmp(loc_token->val, "<=")) {
        return LEQ;
    } else if (dynstr_cmp(loc_token->val, ">=")) {
        return GEQ;
    } else if (dynstr_cmp(loc_token->val, "<")) {
        return LTN;
    } else if (dynstr_cmp(loc_token->val, ">")) {
        return GTN;
    } else if (dynstr_cmp(loc_token->val, "&&")) {
        return LAND;
    } else if (dynstr_cmp(loc_token->val, "||")) {
        return LOR;
    } else if (dynstr_cmp(loc_token->val, "??")) {
        return DBLQ;
    } else if (dynstr_cmp(loc_token->val, "(")) {
        return BRACKETS;
    } else if (dynstr_cmp(loc_token->val, ")")) {
        return BRACKETE;
    } else if (loc_token->dtype == boolT) {
        return BOOLVAR;
    } else if ((loc_token->dtype == intnumT) || (loc_token->dtype == doublenumT) || (loc_token->dtype == stringT)) {
        return VALUE;
    } else if ((loc_token->dtype == varidT)) {
        return IDENTIFIER;
    } else {
        TakeToken = false;
        return DOLLAR;
    }
}
// TO~DO Strom na semantiku
int Expression(struct bst_tok_node **seed, char *EOE, bst_node **sym_table) {

    table_symbol_enum inputed_symbol, top;
    table_sign_enum operation;
    stack_item *ptr;
    stack *prec_stack = malloc(sizeof(stack));
    stack_init(prec_stack);
    stack_push(prec_stack, DOLLAR, NULL);
    int iteration = 0;
    bool functi_switch = false;
    table_symbol_enum stack_index;

    // buffer for rules
    dyn_string buffer;
    dynstr_init(&buffer);

    GetToken();
    do {
        // GET INPUT SYMBOL
        inputed_symbol = get_table_symbol(myToken, NULL);
        // HELP PRINT
        // printf("%d\n", inputed_symbol);

        // GET OPERATION

        // IF E SKIP IT
        if (prec_stack->top->symbol == ENTERPRISE) {
            stack_index = prec_stack->top->next->symbol;
        } else {
            stack_index = prec_stack->top->symbol;
        }
        operation = prec_table[get_index_enum(stack_index)][get_index_enum(inputed_symbol)];

        // CHECK PRINT
        // printf("Operation :%d FOR STACK:%d AND INPUT:%d\n", operation, get_index_enum(stack_index), get_index_enum(inputed_symbol));

        switch (operation) {
            case S:
                // Check if E is on top if is push between, else push on top
                if (stack_top(prec_stack)->symbol == ENTERPRISE) {
                    item_push(prec_stack, SHIFT, prec_stack->top->next);
                    stack_push(prec_stack, inputed_symbol, myToken);
                } else {
                    stack_push(prec_stack, SHIFT, NULL);
                    stack_push(prec_stack, inputed_symbol, myToken);
                }
                GetToken();
                break;
            case N:
                stack_push(prec_stack, inputed_symbol, myToken);
                GetToken();
                break;
            case E:
                return false;
                break;
            case R:
                // FILL BUFFER
                ptr = stack_top(prec_stack);
                top = stack_top(prec_stack)->symbol;
                while (top != SHIFT) {
                    if ((top == IDENTIFIER) || (top == VALUE)) {
                        dynstr_add(&buffer, 'i');
                    } else if (top == ENTERPRISE) {
                        dynstr_add(&buffer, 'E');
                    } else if (top == NOT_NOTNIL) {
                        dynstr_add(&buffer, '!');
                    } else if (top == MUL) {
                        dynstr_add(&buffer, '*');
                    } else if (top == DIV) {
                        dynstr_add(&buffer, '/');
                    } else if (top == PLUS) {
                        dynstr_add(&buffer, '+');
                    } else if (top == MINUS) {
                        dynstr_add(&buffer, '-');
                    } else if (top == EQ) {
                        dynstr_addstr(&buffer, "==");
                    } else if (top == NEQ) {
                        dynstr_addstr(&buffer, "!=");
                    } else if (top == LEQ) {
                        dynstr_addstr(&buffer, "<=");
                    } else if (top == LTN) {
                        dynstr_addstr(&buffer, "<");
                    } else if (top == GEQ) {
                        dynstr_addstr(&buffer, ">=");
                    } else if (top == GTN) {
                        dynstr_addstr(&buffer, ">");
                    } else if (top == DBLQ) {
                        dynstr_addstr(&buffer, "??");
                    } else if (top == BRACKETS) {
                        dynstr_add(&buffer, '(');
                    } else if (top == BRACKETE) {
                        dynstr_add(&buffer, ')');
                    } else if (top == LAND) {
                        dynstr_addstr(&buffer, "&&");
                    } else if (top == LOR) {
                        dynstr_addstr(&buffer, "||");
                    } else {
                        exit(69);
                    }
                    ptr = ptr->next;
                    top = ptr->symbol;
                }
                // dynstr_print(&buffer);
                //  Pick Rule
                if (dynstr_cmp(&buffer, "i")) {
                    if (prec_stack->top->tok_node->T->dtype == varidT) {
                        // SEMANTIC~CHECK
                        Insert_BTree(sym_table, prec_stack->top->tok_node->T->val->s, prec_stack->top->tok_node->T->dtype, false, !functi_switch);
                        functi_switch = false;
                    }
                    stack_top(prec_stack)->symbol = ENTERPRISE;
                } else if (dynstr_cmp(&buffer, "E+E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E-E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E*E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E/E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, ")E(")) {
                    stack_pop(prec_stack);
                    stack_pop_UndTop(prec_stack);
                } else if (dynstr_cmp(&buffer, "E==E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E!=E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E<E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E<=E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E>E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E>=E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E??E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E&&E")) {
                    T_Body(prec_stack);
                } else if (dynstr_cmp(&buffer, "E||E")) {
                    T_Body(prec_stack);
                }
                // NOTNIL
                else if (dynstr_cmp(&buffer, "!E")) {
                    stack_pop(prec_stack);
                }
                // NEGATION
                else if (dynstr_cmp(&buffer, "E!")) {
                    struct bst_tok_node *parent = prec_stack->top->tok_node;
                    stack_pop(prec_stack);
                    struct bst_tok_node *LeftChild = prec_stack->top->tok_node;
                    stack_pop(prec_stack);
                    struct bst_tok_node *root = SetChildNodes(parent, LeftChild, NULL);
                    stack_push_node(prec_stack, ENTERPRISE, root);

                }
                // TODO pridat ?? a !
                else {
                    exit(2);
                }
                stack_pop_UndTop(prec_stack);
                dynstr_clear(&buffer);
                break;
            case F:
                FunctionCall(&prec_stack->top->tok_node->left, sym_table);
                functi_switch = true;
                GetToken();
            default:
                break;
        }
        // DEL stack_push(prec_stack, inputed_symbol, myToken);

        // stack_print(prec_stack->top);
        iteration++;

        if (prec_stack->top->symbol == ENTERPRISE) {
            stack_index = prec_stack->top->next->symbol;
        } else {
            stack_index = prec_stack->top->symbol;
        }
    } while (((inputed_symbol != DOLLAR) || (stack_index != DOLLAR)));
    (*seed) = prec_stack->top->tok_node;
    stack_free(prec_stack);
    return true;
}

void T_Body(stack *my_stack) {
    if (my_stack->top->symbol == BRACKETS) {
        stack_pop(my_stack);
        stack_pop_UndTop(my_stack);
    } else {
        struct bst_tok_node *tmpNode1 = my_stack->top->tok_node;
        stack_pop(my_stack);
        struct bst_tok_node *tmpNode2 = my_stack->top->tok_node;
        stack_pop(my_stack);
        struct bst_tok_node *tmpNode3 = my_stack->top->tok_node;
        stack_pop(my_stack);
        struct bst_tok_node *root = SetChildNodes(tmpNode2, tmpNode1, tmpNode3);
        stack_push_node(my_stack, ENTERPRISE, root);
    }
}