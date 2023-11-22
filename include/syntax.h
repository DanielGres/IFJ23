// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#ifndef SYNTAX_H
#define SYNTAX_H

#include "lexer.h"
#include "tok_tree.h"
#include "expression.h"

bool EndCommand();

void EnterSkip();

bool CorpusPrime(struct bst_tok_node **seed);

bool CorpusSecondary(struct bst_tok_node **seed);

bool Expression_SetUp(struct bst_tok_node **seed, expression_case type);

bool Let(struct bst_tok_node **seed);

bool IfPrime(struct bst_tok_node **seed);

#endif