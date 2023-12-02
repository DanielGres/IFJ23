// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13

#include "syntax.h"

token *myToken;
bool TakeToken = true;

/**
 * @brief Function takes token from lexer if TakeToken is true,
 * otherwise it returns the same used token
 *
 */
#define GetToken()           \
    if (TakeToken) {         \
        Get_Token(&myToken); \
    } else {                 \
        TakeToken = true;    \
    }

// bool Expression(struct bst_tok_node **seed) {
//     GetToken();
//     if (myToken->dtype != intnumT) return false;
//     *seed = Set_TokNode(myToken);
//     return true;
// }

bool CorpusPrime(struct bst_tok_node **seed, bst_node **sym_table) {
    GetToken();
    switch (myToken->dtype) {
        case letT: {
            *seed = Set_TokNode(myToken);
            if (!Let(&((*seed)->left), sym_table)) return false;
            if (!EndCommand()) return false;
            return CorpusPrime(&((*seed)->right), sym_table);
        } break;
        case varT: {
            *seed = Set_TokNode(myToken);
            if (!Var(&((*seed)->left), sym_table)) return false;
            if (!EndCommand()) return false;
            return CorpusPrime(&((*seed)->right), sym_table);
        } break;
        case varidT: {
            *seed = Set_TokNode(myToken);
            GetToken();
            if (myToken->dtype == equalT) {
                if (!assigment(&((*seed)->left), sym_table)) return false;
            } else if (myToken->dtype == LbracketT) {
                // SEMANTIC~CHECK
                Insert_BTree(sym_table, (*seed)->T->val->s, (*seed)->T->dtype, false, false);

                if (!FunctionCall(&((*seed)->left), sym_table)) return false;
            } else {
                return false;
            }

            if (!EndCommand()) return false;
            return CorpusPrime(&((*seed)->right), sym_table);
        } break;
        case ifT: {
            *seed = Set_TokNode(myToken);
            if (!IfPrime(&((*seed)->left), sym_table)) return false;
            return CorpusPrime(&((*seed)->right), sym_table);
        } break;
        case whileT: {
            *seed = Set_TokNode(myToken);
            if (!WhilePrime(&((*seed)->left), sym_table)) return false;
            return CorpusPrime(&((*seed)->right), sym_table);
        } break;
        case funcT: {
            *seed = Set_TokNode(myToken);
            if (!FunctionDef(&((*seed)->left), sym_table)) return false;
            return CorpusPrime(&((*seed)->right), sym_table);
        } break;
        case eofT: {
            return true;
        } break;
        case linecommentT:
        case newlineT: {
            return CorpusPrime(&(*seed), sym_table);
        } break;
        default: {
            return false;
        }
    }
}

bool CorpusSecondary(struct bst_tok_node **seed, bst_node **sym_table) {
    GetToken();
    switch (myToken->dtype) {
        case letT: {
            *seed = Set_TokNode(myToken);
            if (!Let(&((*seed)->left), sym_table)) return false;
            if (!EndCommand()) return false;
            return CorpusSecondary(&((*seed)->right), sym_table);
        } break;
        case varT: {
            *seed = Set_TokNode(myToken);
            if (!Var(&((*seed)->left), sym_table)) return false;
            if (!EndCommand()) return false;
            return CorpusSecondary(&((*seed)->right), sym_table);
        } break;
        case varidT: {
            *seed = Set_TokNode(myToken);
            GetToken();
            if (myToken->dtype == equalT) {
                if (!assigment(&((*seed)->left), sym_table)) return false;
            } else if (myToken->dtype == LbracketT) {
                // SEMANTIC~CHECK
                Insert_BTree(sym_table, (*seed)->T->val->s, (*seed)->T->dtype, false, false);

                if (!FunctionCall(&((*seed)->left), sym_table)) return false;
            } else {
                return false;
            }

            if (!EndCommand()) return false;
            return CorpusSecondary(&((*seed)->right), sym_table);
        } break;
        case ifT: {
            *seed = Set_TokNode(myToken);
            if (!IfPrime(&((*seed)->left), sym_table)) return false;
            return CorpusSecondary(&((*seed)->right), sym_table);
        } break;
        case whileT: {
            *seed = Set_TokNode(myToken);
            if (!WhilePrime(&((*seed)->left), sym_table)) return false;
            return CorpusSecondary(&((*seed)->right), sym_table);
        } break;
        case returnT:
            // Chytit variable dat ju do return stroma
            GetToken();
            if(myToken->dtype != varidT) return false;
            return CorpusSecondary(&(*seed), sym_table);
        case RCbracketT: {
            return true;
        } break;
        case linecommentT:
        case newlineT: {
            return CorpusSecondary(&(*seed), sym_table);
        } break;
        default: {
            return false;
        }
    }
}

bool EndCommand() {
    GetToken();
    switch (myToken->dtype) {
        case semicolonT: {
            GetToken();
            if (myToken->dtype == newlineT)
                return true;
            else {
                TakeToken = false;
                return true;
            }
        } break;
        case newlineT: {
            return true;
        } break;
        case eofT: {
            TakeToken = false;
            return true;
        } break;
        default: {
            return false;
        }
    }
}

void EnterSkip() {
    if (myToken->dtype == newlineT) {
        GetToken();
    }
}

bool FunctionDef(struct bst_tok_node **seed, bst_node **sym_table) {
    GetToken();
    *seed = Set_TokNode(myToken);
    if (myToken->dtype != varidT) return false;

    // SEMANTIC~CHECK
    Insert_BTree(sym_table, (*seed)->T->val->s, (*seed)->T->dtype, true, false);

    // MAJKOV HOKUS POKUS
    bst_node *ptr = SubTreePointer((*sym_table), (*seed)->T->val->s);

    GetToken();
    (*seed)->left = Set_TokNode(myToken);
    if (myToken->dtype != LbracketT) return false;
    if (!FunctionDefParams(&((*seed)->left->left), sym_table, (*seed)->T->val->s)) return false;
    GetToken();
    if (myToken->dtype == arrowT) {
        GetToken();
        if (myToken->dtype != vartypeT) return false;
        (*seed)->right = Set_TokNode(myToken);
        GetToken();
        if (myToken->dtype != LCbracketT) return false;
        return CorpusSecondary(&((*seed)->left->right), &ptr->funcTree);
    } else if (myToken->dtype == LCbracketT) {
        return CorpusSecondary(&((*seed)->left->right), &ptr->funcTree);
    } else {
        return false;
    }
    return true;
}

bool FunctionDefParams(struct bst_tok_node **seed, bst_node **sym_table, char *name) {
    GetToken();
    switch (myToken->dtype) {
        case RbracketT: {
            return true;
        } break;
        case underscoreT:
        case varidT: {
            *seed = Set_TokNode(myToken);
            GetToken();
            if (myToken->dtype != varidT) {
                return false;
            }
            (*seed)->right = Set_TokNode(myToken);
            GetToken();
            if (myToken->dtype != colonT) {
                return false;
            }
            GetToken();
            if (myToken->dtype != vartypeT) {
                return false;
            }
            Insert_FuncSubTree(sym_table, name, (*seed)->right->T->val->s, (*seed)->right->T->dtype, true, true);
            (*seed)->right->right = Set_TokNode(myToken);

            GetToken();
            if (myToken->dtype == commaT) {
                return FunctionDefParams(&((*seed)->left), sym_table, name);
            } else if (myToken->dtype == RbracketT) {
                return true;
            } else {
                return false;
            }
        } break;
        default: {
            return false;
        }
    }
}

// Semantics should check if the variable was declared before and if it is modifiable
bool assigment(struct bst_tok_node **seed, bst_node **sym_table) {
    return Expression(&((*seed)), NULL, sym_table);
}

bool FunctionCall(struct bst_tok_node **seed, bst_node **sym_table) {
    // This sets up "(" to the left of varidT so code generation knows its funciton call
    *seed = Set_TokNode(myToken);
    // Check for parameters and set them to the left

    return FunctionCallParameters(&((*seed)->left), sym_table);
}

bool FunctionCallParameters(struct bst_tok_node **seed, bst_node **sym_table) {
    GetToken();
    switch (myToken->dtype) {
        case RbracketT: {
            return true;
        } break;
        case intnumT:
        case doublenumT:
        case stringT: {
            *seed = Set_TokNode(myToken);
            GetToken();
            if (myToken->dtype == commaT) {
                return FunctionCallParameters(&((*seed)->left), sym_table);
            } else if (myToken->dtype == RbracketT) {
                return true;
            } else {
                return false;
            }
        } break;
        case varidT: {
            *seed = Set_TokNode(myToken);

            GetToken();
            if (myToken->dtype == colonT) {
                GetToken();
                if (myToken->dtype != varidT) {
                    return false;
                }
                (*seed)->right = Set_TokNode(myToken);
                // SEMANTIC~CHECK
                Insert_BTree(sym_table, (*seed)->right->T->val->s, (*seed)->right->T->dtype, false, true);
                GetToken();
                if (myToken->dtype == commaT) {
                    return FunctionCallParameters(&((*seed)->left), sym_table);
                } else if (myToken->dtype == RbracketT) {
                    return true;
                } else {
                    return false;
                }
            } else if (myToken->dtype == commaT) {
                return FunctionCallParameters(&((*seed)->left), sym_table);
            } else if (myToken->dtype == RbracketT) {
                return true;
            } else {
                return false;
            }
        } break;
        default: {
            return false;
        }
    }
}

// Let will be stored as a constant ( semantics should check if that variable is being modified )
bool Let(struct bst_tok_node **seed, bst_node **sym_table) {
    GetToken();
    *seed = Set_TokNode(myToken);
    if (myToken->dtype != varidT) return false;

    // SEMANTIC~CHECK
    Insert_BTree(sym_table, (*seed)->T->val->s, (*seed)->T->dtype, true, true);

    GetToken();
    switch (myToken->dtype) {
        case colonT: {
            GetToken();
            if (myToken->dtype != vartypeT && myToken->dtype != vartypeQT) return false;
            GetToken();
            if (myToken->dtype == equalT) {
                return Expression(&((*seed)->left), NULL, sym_table);
            } else {
                TakeToken = false;
                return true;
            }
        } break;
        case equalT: {
            return Expression(&((*seed)->left), NULL, sym_table);
        } break;
        default: {
            return false;
        }
    }
}

// Var will be allowed to change its value ( by semantics )
bool Var(struct bst_tok_node **seed, bst_node **sym_table) {
    GetToken();
    *seed = Set_TokNode(myToken);
    if (myToken->dtype != varidT) return false;

    // SEMANTIC~CHECK
    Insert_BTree(sym_table, (*seed)->T->val->s, (*seed)->T->dtype, true, true);

    GetToken();
    switch (myToken->dtype) {
        case colonT: {
            GetToken();
            if (myToken->dtype != vartypeT && myToken->dtype != vartypeQT) return false;
            GetToken();
            if (myToken->dtype == equalT) {
                return Expression(&((*seed)->left), NULL, sym_table);
            } else {
                TakeToken = false;
                return true;
            }
        } break;
        case equalT: {
            return Expression(&((*seed)->left), NULL, sym_table);
        } break;
        default: {
            return false;
        }
    }
}

bool IfPrime(struct bst_tok_node **seed, bst_node **sym_table) {
    *seed = Set_TokNode(myToken);
    if (!Expression(&((*seed)->left), NULL, sym_table)) return false;
    GetToken();
    EnterSkip();
    if (myToken->dtype != LCbracketT) return false;
    ((*seed)->right) = Set_TokNode(myToken);
    // IF part goes to left
    if (!CorpusSecondary(&((*seed)->right)->left, sym_table)) return false;
    GetToken();
    EnterSkip();
    if (myToken->dtype != elseT) return false;
    GetToken();
    EnterSkip();
    if (myToken->dtype != LCbracketT) return false;
    // ELSE part goes to right
    if (!CorpusSecondary(&((*seed)->right)->right, sym_table)) return false;
    return true;
}

bool WhilePrime(struct bst_tok_node **seed, bst_node **sym_table) {
    *seed = Set_TokNode(myToken);
    if (!Expression(&((*seed)->left), NULL, sym_table)) return false;
    GetToken();
    EnterSkip();
    if (myToken->dtype != LCbracketT) return false;
    ((*seed)->right) = Set_TokNode(myToken);
    if (!CorpusSecondary(&((*seed)->right)->right, sym_table)) return false;
    return true;
}