// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#ifndef SYNTAX_H
#define SYNTAX_H

#include "lexer.h"
#include "bin_tree.h"

bool EndCommand();

void EnterSkip();

/**
 * @brief Main program block syntax analyser,
 *  recursively calls itself and other functions until end of file
 * 
 * @param seed 
 * @return true 
 * @return false 
 */
bool CorpusPrime(struct bst_tok_node **seed);

bool CorpusSecondary(struct bst_tok_node **seed);

bool Expression();

bool Let(struct bst_tok_node **seed);

bool Var(struct bst_tok_node **seed);

bool assigment(struct bst_tok_node **seed);

bool IfPrime(struct bst_tok_node **seed);

bool FuncDef(struct bst_tok_node **seed);

#endif