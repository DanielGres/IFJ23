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

    // // branov lexing testing fosat geci kod zakomentuj nemaz more TODO
    // while(Get_Token(&T)){
    //     dynstr_print((T->val));
    //     printf(" %s\n",enumers[T->dtype]);
    // }

    if(CorpusPrime(&(the_root->right))){
        printf("spravne! :DD\n");
        PostorderTraversal(the_root->right);
    }
    else{
        printf("nespravne! :((\n");
    }

    // if (Expression(the_root->left, ";")) {
    //     printf("spravne! :DD\n");
    //     inorderTraversal(the_root->left);
    //     // dynstr_print(&the_root->right->right->T->val);

    // } else {
    //     printf("nespravne! :((\n");
    // }
    // printf("\n");

    // TEST STACK
    // stack *node_stack = malloc(sizeof(stack));
    // stack_init(node_stack);
    // while (Get_Token(&T)) {
    //     dynstr_print(T->val);
    //     stack_push(node_stack, ENTERPRISE, T);
    //     // printf("%d\n", node_stack->top->symbol);
    // }
    // item_push(node_stack, SHIFT, node_stack->top->next);
    // stack_print(node_stack->top);
    // stack_pop_UndTop(node_stack);
    // stack_pop_UndTop(node_stack);
    // stack_print(node_stack->top);
    // printf("\n");
    // stack_print(node_stack->top);
    // T_Body(node_stack);
    // stack_print(node_stack->top);
    // dynstr_print(&(node_stack->top->tok_node->T->val));
    // stack_free(node_stack);
    // stack_print(node_stack->top);
}