#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>

#include "ast.h"

int main(void) {
    ASTNode *var_decl = CreateVarDeclNode("x");

    ASTNode *l_num = CreateNumLiteralNode(10);
    ASTNode *r_num = CreateNumLiteralNode(5);
    ASTNode *bop_node = CreateBinOpNode(PlusBop, l_num, r_num);
    Context ctx;
    InitVarMap(ctx.var_map, STACK_SIZE);

    ASTNode *ast = CreateBinOpNode(AssignBop, var_decl, bop_node);

    Eval(&ctx, ast);
    printf("Evaluated\n");

    ConvertVarNameToNum(&ctx, &ctx.value);
    printf("eval result = %lli\n", ctx.value.num);

    return 0;
}
