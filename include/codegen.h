// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#ifndef CODEGEN_H
#define CODEGEN_H

#include "tok_tree.h"
#include "token.h"

void Generator(struct bst_tok_node *root);

void GenerateSubTree(struct bst_tok_node *curr_root);

void GenerateExpression(struct bst_tok_node *root);

void GenerateIF(struct bst_tok_node *root);

void GenerateLet(struct bst_tok_node *root);

void GenerateVar(struct bst_tok_node *root);

void CallFuncAssigment(struct bst_tok_node *root);

void GenerateCallFunction(struct bst_tok_node *root);

#endif