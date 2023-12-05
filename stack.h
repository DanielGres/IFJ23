#ifndef STACK_H
#define STACK_H
#include "tok_tree.h"
#include "token.h"

typedef enum {
    NOT_NOTNIL,  // ! - NOT (!boolvar), NOTNIL (var!)
    MUL,         // *
    DIV,         // /
    PLUS,        // +
    MINUS,       // -
    EQ,          // ==
    NEQ,         // !=
    LEQ,         // <=
    LTN,         // <
    GEQ,         // >=
    GTN,         // >
    LAND,        // && logical and
    LOR,         // || logical or
    DBLQ,        // ??
    BRACKETS,    // (
    BRACKETE,    // )
    BOOLVAR,     // bool
    VALUE,       // value
    IDENTIFIER,  // ID (variable, number, string, function)
    DOLLAR,      // $

    ENTERPRISE,  // E-boy
    SHIFT        // Shift
} table_symbol_enum;

typedef struct stack_item {
    table_symbol_enum symbol;
    struct bst_tok_node *tok_node;
    struct stack_item *next;
} stack_item;

typedef struct
{
    stack_item *top;
} stack;

/**
 * @brief initializes stack for bottom up method for syntax analyzer
 *
 * @param precedent_stack pointer to stack
 */
void stack_init(stack *precedent_stack);

/**
 * @brief pushes a new item onto the stack
 *
 * @param precedent_stack pointer to the stack where the new item will be pushed
 * @param symbol specific symbol from table of symbols to determine what type of token is pushed onto the stack
 * @param T token of symbol to be pushed onto the given stack
 * @return true if the function was successful
 * @return false if the function cant allocate memory for given
 */
bool stack_push(stack *precedent_stack, table_symbol_enum symbol, token *T);

/**
 * @brief pushes a new item onto the stack
 *
 * @param precedent_stack pointer to the stack where the new item will be pushed
 * @param symbol specific symbol from table of symbols to determine what type of token is pushed onto the stack
 * @param node
 * @return true if the function was successful
 * @return false if the function cant allocate memory for given
 */
bool stack_push_node(stack *precedent_stack, table_symbol_enum symbol, struct bst_tok_node *node);

/**
 * @brief pushes a new item as the second one right behind the top one
 *
 * @param precedent_stack pointer to the stack where the new item will be pushed
 * @param item_next
 * @param symbol specific symbol from table of symbols to determine what type of token is pushed into the stack
 * @return true
 * @return false
 */
bool item_push(stack *precedent_stack, table_symbol_enum symbol, stack_item *item_next);

// /**
//  * @brief pops a stack
//  *
//  * @param precedent_stack stack
//  * @return true successful
//  * @return false unsuccessful
//  */
bool stack_pop(stack *precedent_stack);

/**
 * @brief prints a stack from item till bottom
 *
 * @param the_item item from which, to print
 */
void stack_print(stack_item *item);

/**
 * @brief pops a stuck x [count] times
 *
 * @param precedent_stack
 * @param count
 */
void stack_pop_count(stack *precedent_stack, int count);

/**
 * @brief compares a item's symbol with symbol
 *
 * @param item1 item to compare
 * @param symbol symbol to compare
 * @return true
 * @return false
 */
bool item_symblcmp(stack_item *item1, table_symbol_enum symbol);

/**
 * @brief returns pointer item on top
 *
 * @param precedent_stack stack
 * @return pointer to item
 */
stack_item *stack_top(stack *precedent_stack);

/**
 * @brief frees stack, doesnt have to be empty
 *
 * @param precedent_stack
 */
void stack_free(stack *precedent_stack);

bool stack_pop_UndTop(stack *precedent_stack);

#endif