// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#ifndef SYNTAX_H
#define SYNTAX_H

#include "expression.h"
#include "lexer.h"
#include "tok_tree.h"

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
bool CorpusPrime(struct bst_tok_node **seed, bst_node **sym_table);

bool CorpusSecondary(struct bst_tok_node **seed, bst_node **sym_table);

bool Let(struct bst_tok_node **seed, bst_node **sym_table);

bool Var(struct bst_tok_node **seed, bst_node **sym_table);

bool assigment(struct bst_tok_node **seed, bst_node **sym_table);

bool FunctionDef(struct bst_tok_node **seed, bst_node **sym_table);

bool FunctionDefParams(struct bst_tok_node **seed, bst_node **sym_table, char *name);

bool FunctionCall(struct bst_tok_node **seed, bst_node **sym_table);

bool FunctionCallParameters(struct bst_tok_node **seed, bst_node **sym_table);

bool IfPrime(struct bst_tok_node **seed, bst_node **sym_table);

bool WhilePrime(struct bst_tok_node **seed, bst_node **sym_table);

bool FuncDef(struct bst_tok_node **seed, bst_node **sym_table);

#endif  // Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
