// Implementace překladače imperativního jazyka IFJ22
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#include "../include/stack.h"

void stack_init(stack *precedent_stack)
{
	precedent_stack->top = NULL;
}

bool stack_push(stack *precedent_stack, table_symbol_enum symbol)
{
	
	stack_item *new_item = malloc(sizeof(stack_item) + 16); ////PORIESIT NESKOR
	if (new_item == NULL)
	{
		return false;
	}

	new_item->symbol = symbol;
	new_item->next = precedent_stack->top;

	precedent_stack->top = new_item;

	return true;
}
bool item_push(stack *precedent_stack, stack_item *item_next, table_symbol_enum symbol)
{
	stack_item *new_item = (stack_item *)malloc(sizeof(stack_item) + 16); // PORIESIT NESKOR

	if (new_item == NULL)
		return false;

	new_item->symbol = symbol;
	new_item->next = item_next;

	precedent_stack->top->next = new_item;

	return true;
}

bool stack_pop(stack *precedent_stack)
{
	if (precedent_stack->top != NULL)
	{
		stack_item *tmp = precedent_stack->top;
		precedent_stack->top = tmp->next;
		free(tmp);

		return true;
	}

	return false;
}

void stack_pop_count(stack *precedent_stack, int count)
{
	for (int i = 0; i < count; i++)
	{
		stack_pop(precedent_stack);
	}
}

stack_item *stack_top(stack *precedent_stack)
{
	return precedent_stack->top;
}

void stack_free(stack *precedent_stack)
{
	while (stack_pop(precedent_stack))
		;
}
void print_stack(stack_item *the_item)
{
	if (the_item != NULL)
	{
		printf("%d ", the_item->symbol);
		print_stack(the_item->next);
	}
	else
	{
		printf("\n");
	}
}
bool item_symblcmp(stack_item *item1, table_symbol_enum symbol)
{
	if (item1 != NULL)
	{
		if (item1->symbol == symbol)
		{
			return true;
		}
	}
	return false;
}

void stack_initT(stack_of_T *token_stack)
{
	token_stack->top = NULL;
}

bool stack_pushT(stack_of_T *token_stack, token *myToken)
{

	stack_item_of_T *new_item = malloc(sizeof(stack_item_of_T) + 24); // PORIESIT NESKOR
	if (new_item == NULL)
	{

		return false;
	}
	Token_init(&(new_item->stackToken));
	dynstr_init(&(new_item->stackToken.val));
	new_item->stackToken.dtype = myToken->dtype;
	dynstr_copy(&(new_item->stackToken.val), myToken->val);
	
	new_item->next = token_stack->top;

	token_stack->top = new_item;
	return true;
}

bool stack_popT(stack_of_T *token_stack)
{
	if (token_stack->top != NULL)
	{
		
		stack_item_of_T *tmp = token_stack->top;
		token_stack->top = tmp->next;
		free(tmp);

		return true;
	}


	return false;
}

stack_item_of_T *stack_topT(stack_of_T *token_stack)
{
	return token_stack->top;
}

void stack_freeT(stack_of_T *token_stack)
{
	while (stack_popT(token_stack))
		;
}
void print_stackT(stack_item_of_T *the_item)
{
	if (the_item != NULL)
	{
		dynstr_print(&(the_item->stackToken.val));
		print_stackT(the_item->next);
	}
	else
	{
		printf("\n");
	}
}

void stack_init_node(stack_of_node *node_stack)
{
	node_stack->top = NULL;
}

bool stack_push_node(stack_of_node *node_stack, struct bst_tok_node *node)
{
	stack_item_of_node *new_item = malloc(sizeof(stack_item_of_node) + 16); ////PORIESIT NESKOR
	if (new_item == NULL)
	{
	
		return false;
	}
	new_item->ptr_to_node = node;
	new_item->next = node_stack->top;
	node_stack->top = new_item;
	return true;
}
bool stack_pop_node(stack_of_node *node_stack)
{
	if (node_stack->top != NULL)
	{
		stack_item_of_node *tmp = node_stack->top;
		node_stack->top = tmp->next;
		free(tmp);

		return true;
	}


	return false;
}

void stack_free_node(stack_of_node *node_stack)
{
	while (stack_pop_node(node_stack))
		;
}

stack_item_of_T *prevKaffee(stack_of_T *my_stack)
{
	return &my_stack->top->next;
}