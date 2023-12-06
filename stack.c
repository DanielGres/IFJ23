/**
 * @file stack.c
 * @author xgresd00, xmihal13
 * @brief
 * @version 0.1
 * @date 2023-12-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "stack.h"

void stack_init(stack *precedent_stack) {
    precedent_stack->top = NULL;
}

bool stack_push(stack *precedent_stack, table_symbol_enum symbol, token *T) {
    stack_item *new_item = malloc(sizeof(stack_item) + 32);
    if (new_item == NULL) {
        return false;
    }

    (new_item)->symbol = symbol;
    (new_item)->tok_node = Set_TokNode(T);
    (new_item)->next = precedent_stack->top;
    precedent_stack->top = new_item;

    return true;
}
bool stack_push_node(stack *precedent_stack, table_symbol_enum symbol, struct bst_tok_node *node) {
    stack_item *new_item = malloc(sizeof(stack_item) + 16);
    if (new_item == NULL) {
        return false;
    }
    new_item->tok_node = node;
    new_item->symbol = symbol;
    new_item->next = precedent_stack->top;
    precedent_stack->top = new_item;
    return true;
}

bool item_push(stack *precedent_stack, table_symbol_enum symbol, stack_item *item_next) {
    stack_item *new_item = (stack_item *)malloc(sizeof(stack_item) + 16);

    if (new_item == NULL)
        return false;

    new_item->symbol = symbol;
    new_item->next = item_next;

    precedent_stack->top->next = new_item;

    return true;
}

bool stack_pop(stack *precedent_stack) {
    if (precedent_stack->top != NULL) {
        stack_item *tmp = precedent_stack->top;
        precedent_stack->top = tmp->next;
        free(tmp);

        return true;
    }

    return false;
}

void stack_pop_till(stack *precedent_stack, int till) {
    for (int i = 0; i < till; i++) {
        stack_pop(precedent_stack);
    }
}

bool stack_pop_UndTop(stack *precedent_stack) {
    if ((precedent_stack->top != NULL) && (precedent_stack->top->next != NULL)) {
        stack_item *tmp = precedent_stack->top->next;
        precedent_stack->top->next = tmp->next;
        free(tmp);

        return true;
    }
    return false;
}

stack_item *stack_top(stack *precedent_stack) {
    return precedent_stack->top;
}

void stack_free(stack *precedent_stack) {
    while (stack_pop(precedent_stack))
        ;
    free(precedent_stack->top);
}

void stack_print(stack_item *item) {
    if (item != NULL) {
        printf("%d ", item->symbol);
        stack_print(item->next);
    } else {
        printf("\n");
    }
}

bool item_symblcmp(stack_item *item, table_symbol_enum symbol) {
    if (item != NULL) {
        if (item->symbol == symbol) {
            return true;
        }
    }
    return false;
}