#ifndef STACK_H
#define STACK_H
#include "token.h"
#include "bin_tree.h"

typedef enum
{
    NOTNIL,     // !
    PLUS,       // +
    MINUS,      // -
    MUL,        // *
    DIV,        // /
    EQ,         // ===
    NEQ,        // !==
    LEQ,        // <=
    LTN,        // <
    GEQ,        // >=
    GTN,        // >
    BRACKETS,   //	(
    BRACKETE,   //	)
    IDENTIFIER, // ID (variable, number, string, function?)
    DOLLAR,     // $
    ENTERPRISE, // E-boy
    SHIFT       // Shift
} table_symbol_enum;

typedef struct
{
    table_symbol_enum symbol;
    struct stack_item *next;
} stack_item;

typedef struct
{
    stack_item *top;
} stack;

/**
 * @brief
 *
 * @param precedent_stack
 */
void stack_init(stack *precedent_stack);

/**
 * @brief
 *
 * @param precedent_stack
 * @param symbol
 * @return true
 * @return false
 */
bool stack_push(stack *precedent_stack, table_symbol_enum symbol);

/**
 * @brief
 *
 * @param precedent_stack
 * @param item_next
 * @param symbol
 * @return true
 * @return false
 */
bool item_push(stack *precedent_stack, stack_item *item_next, table_symbol_enum symbol);

/**
 * @brief
 *
 * @param precedent_stack
 * @return true
 * @return false
 */
bool stack_pop(stack *precedent_stack);

/**
 * @brief
 *
 * @param precedent_stack
 * @param count
 */
void stack_pop_count(stack *precedent_stack, int count);

/**
 * @brief
 *
 * @param item1
 * @param symbol
 * @return true
 * @return false
 */
bool item_symblcmp(stack_item *item1, table_symbol_enum symbol);

/**
 * @brief
 *
 * @param precedent_stack
 * @return stack_item*
 */
stack_item *stack_top(stack *precedent_stack);

/**
 * @brief
 *
 * @param precedent_stack
 */
void stack_free(stack *precedent_stack);

/**
 * @brief
 *
 * @param the_item
 */
void print_stack(stack_item *the_item);

// STACKS OF TOKENS

typedef struct
{
    token stackToken;
    struct stack_item_of_T *next;
} stack_item_of_T;

typedef struct
{
    stack_item_of_T *top;
} stack_of_T;

/**
 * @brief
 *
 * @param token_stack
 */
void stack_initT(stack_of_T *token_stack);

/**
 * @brief
 *
 * @param token_stack
 * @param myToken
 * @return true
 * @return false
 */
bool stack_pushT(stack_of_T *token_stack, token *myToken);

/**
 * @brief
 *
 * @param token_stack
 * @return true
 * @return false
 */
bool stack_popT(stack_of_T *token_stack);

/**
 * @brief
 *
 * @param token_stack
 * @return stack_item_of_T*
 */
stack_item_of_T *stack_topT(stack_of_T *token_stack);

/**
 * @brief
 *
 * @param token_stack
 */
void stack_freeT(stack_of_T *token_stack);

/**
 * @brief
 *
 * @param the_item
 */
void print_stackT(stack_item_of_T *the_item);

// STACK OF TREE POINTERS

typedef struct
{
    struct bst_tok_node *ptr_to_node;
    struct stack_item_of_node *next;
} stack_item_of_node;

typedef struct
{
    stack_item_of_node *top;
} stack_of_node;

/**
 * @brief
 *
 * @param node_stack
 */
void stack_init_node(stack_of_node *node_stack);

/**
 * @brief
 *
 * @param node_stack
 * @param node
 * @return true
 * @return false
 */
bool stack_push_node(stack_of_node *node_stack, struct bst_tok_node *node);

/**
 * @brief
 *
 * @param node_stack
 * @return true
 * @return false
 */
bool stack_pop_node(stack_of_node *node_stack);

/**
 * @brief
 *
 * @param node_stack
 */
void stack_free_node(stack_of_node *node_stack);

/**
 * @brief
 *
 * @param the_item
 */
void print_stack_of_node(stack_item_of_node *the_item);

/**
 * @brief
 *
 * @param my_stack
 * @return stack_item_of_T*
 */
stack_item_of_T *prevKaffee(stack_of_T *my_stack);

#endif