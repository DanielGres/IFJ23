#include <stdio.h>
#include <stdbool.h>
#include "token.h"
#include "bin_tree.h"
#include "lexer.h"

bool EndCommand();

void EnterSkip();

bool CorpusPrime(struct bst_tok_node **seed);

bool CorpusSecondary(struct bst_tok_node **seed);

bool Expression();

bool Let(struct bst_tok_node **seed);

bool IfPrime(struct bst_tok_node **seed);