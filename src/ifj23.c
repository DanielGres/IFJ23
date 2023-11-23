// Implementace překladače imperativního jazyka IFJ23
// Daniel Greš --- xgresd00
// Mário Mihál --- xmihal13
// Viktor Hančovský --- xhanco00
// Branislav Kotúč --- xkotuc02

#include "../include/ifj23.h"

// TODO na lexer testovanie dik pls nechaj sem abo zakomentuj lem
const char *enumers[] = {
    "varidT",        // variable and function names
    "operatorT",     // !  * /  + -  == != < >  <= >=  ??
    "colonT",        // :
    "vartypeT",      // Double, Int, String or same with ? on the end
    "vartypeQT",     // Double?, Int?, String?
    "equalT",        // =
    "intnumT",       // whole number
    "doublemumT",    // double/float number
    "varT",          // var keyword
    "letT",          // let keyword
    "ifT",           // if keyword
    "stringT",       // string in the input code
    "nilT",          // nil keyword
    "LbracketT",     // (
    "RbracketT",     // )
    "LCbracketT",    // {
    "RCbracketT",    // }
    "elseT",         // else keyword
    "funcT",         // func keyword
    "paramNameT",    // name of the parameter in a function
    "commaT",        // ,
    "arrowT",        // ->
    "whileT",        // while keyword
    "returnT",       // return keyword
    "termT",         // TODO realne hocico to moze byt
    "newlineT",      // \n
    "semicolonT",    // ;
    "linecommentT",  // //comment
    "blockcommentT"  //  /* comment */
};

int main() {
    dyn_string string;
    token *T;
    Token_init(&T);
    dynstr_init(&string);
    dynstr_addstr(&string, "START");
    Token_set(&T, &string, startT);
    struct bst_tok_node *the_root;
    the_root = Set_TokNode(T);

    if (CorpusPrime(&(the_root->right))) {

        Generator(the_root);
    } else {
        printf("nespravne! :((\n");
    }
}