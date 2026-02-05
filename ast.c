#include "ast.h"

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitVarMap(VarPair *var_map, int len) {
    for (int i = 0; i < len; ++i) {
        var_map[i].var_name = NULL;
        var_map[i].var_value = 0;
    }
}

int64_t FindValue(VarPair *var_map, int len, char *key) {
    for (int i = 0; i < len && var_map[i].var_name != NULL; ++i) {
        if (strcmp(var_map[i].var_name, key) == 0) {
            return var_map[i].var_value;
        }
    }

    printf("Using invalid variable '%s'\n", key);
    exit(1);
}

int AddValue(VarPair *var_map, int len, char* name) {
    for (int i = 0; i < len; ++i) {
        if (var_map[i].var_name == NULL) {
            var_map[i].var_name = name;
            return i;
        }
        if (strcmp(var_map[i].var_name, name) == 0) {
            printf("Can not declare two variable with same name '%s'\n", name);
            exit(1);
        }
    }
    printf("Stack overflow\n");
    exit(1);
}

void AssignValue(VarPair *var_map, int len, char *key, int64_t value) {
    for (int i = 0; i < len; ++i) {
        if (var_map[i].var_name == NULL) {
            exit(1);
        }
        if (strcmp(var_map[i].var_name, key) == 0) {
            var_map[i].var_value = value;
            return;
        }
    }
    printf("Unknown variable '%s'\n", key);
    exit(1);
}

void EvalFunctionCall(Context *ctx, ASTNode *node) {
    if (strcmp(node->func_call.func_name, "print") == 0) {
        size_t sz = node->func_call.size;
        ASTNode **args = node->func_call.arguments;

        for (size_t i = 0; i < sz; ++i) {
            Eval(ctx, args[i]);
            ConvertVarNameToNum(ctx, &ctx->value);
            printf("%llu", ctx->value.num);
            if (i == sz - 1) {
                printf("\n");
            } else {
                printf(", ");
            }
        }
        return;
    }

    printf("Unknown function '%s'\n", node->func_call.func_name);
    exit(1);
}

void ConvertVarNameToNum(Context *ctx, CalculactedValue *calculacted_value) {
    if (calculacted_value->value_type != VarName) return;
    calculacted_value->value_type = Num;
    calculacted_value->num = FindValue(ctx->var_map, STACK_SIZE, calculacted_value->var_name);
}

void Eval(Context *ctx, ASTNode *node) {
    switch (node->node_type) {
        case BinOpNodeType: {
            Eval(ctx, node->binop.lhs);
            CalculactedValue lhs = ctx->value;
            
            Eval(ctx, node->binop.rhs);
            CalculactedValue rhs = ctx->value;

            switch (node->binop.bop_id) {
                case PlusBop: {
                    ConvertVarNameToNum(ctx, &lhs);
                    ConvertVarNameToNum(ctx, &rhs);
                    ctx->value.value_type = Num;
                    ctx->value.num = lhs.num + rhs.num;
                    break;
                }
                case MinusBop: {
                    ConvertVarNameToNum(ctx, &lhs);
                    ConvertVarNameToNum(ctx, &rhs);
                    ctx->value.value_type = Num;
                    ctx->value.num = lhs.num - rhs.num;
                    break;
                }
                case MultBop: {
                    ConvertVarNameToNum(ctx, &lhs);
                    ConvertVarNameToNum(ctx, &rhs);
                    ctx->value.value_type = Num;
                    ctx->value.num = lhs.num * rhs.num;
                    break;
                }
                case DevideBop: {
                    ConvertVarNameToNum(ctx, &lhs);
                    ConvertVarNameToNum(ctx, &rhs);;
                    ctx->value.value_type = Num;
                    ctx->value.num = lhs.num / rhs.num;
                    break;
                }
                case AssignBop: {
                    ConvertVarNameToNum(ctx, &rhs);
                    if (lhs.value_type != VarName) {
                        printf("Can not assign value to rvalue\n");
                        exit(1);
                    }
                    AssignValue(ctx->var_map, STACK_SIZE, lhs.var_name, rhs.num);
                    ctx->value.value_type = VarName;
                    ctx->value.var_name = lhs.var_name;
                    break;
                }
            }


            break;
        }
        case NumLiteralNodeType: {
            ctx->value.value_type = Num;
            ctx->value.num = node->num_literal;
            break;
        }
        case VarDeclNodeType: {
            AddValue(ctx->var_map, STACK_SIZE, node->var_name);
            ctx->value.value_type = VarName;
            ctx->value.var_name = node->var_name;
            break;
        }
        case FuncCallNodeType: {
            EvalFunctionCall(ctx, node);
            break;
        }
        case VariableNodeType: {
            ctx->value.value_type = VarName;
            ctx->value.var_name = node->var_name;
            break;
        }
    }
}

ASTNode* CreateBinOpNode(int bop_id, ASTNode *lhs, ASTNode *rhs) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->node_type = BinOpNodeType;
    node->binop.bop_id = bop_id;
    node->binop.lhs = lhs;
    node->binop.rhs = rhs;
    return node;
}

ASTNode* CreateNumLiteralNode(int64_t num_literal) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->node_type = NumLiteralNodeType;
    node->num_literal = num_literal;
    return node;
}

ASTNode* CreateVarDeclNode(char *var_name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->node_type = VarDeclNodeType;
    node->var_name = var_name;
    return node;
}

ASTNode* CreateFuncCallNode(char *func_name, ASTNode **args, size_t sz) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->node_type = FuncCallNodeType;
    node->func_call.func_name = func_name;
    node->func_call.arguments = args;
    node->func_call.size = sz;
    return node;
}

ASTNode* CreateVariableNode(char *name) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->node_type = VariableNodeType;
    node->var_name = name;
    return node;
}
