#pragma once

#include <stddef.h>
#include <stdint.h>

#define STACK_SIZE 1024

typedef struct ASTNode ASTNode;

typedef struct ASTNode {
    enum {
        BinOpNodeType,
        NumLiteralNodeType,
        VarDeclNodeType,
        FuncCallNodeType,
        VariableNodeType,
    } node_type;

    union {
        struct {
            enum {
                PlusBop,
                MinusBop,
                AssignBop,
                DevideBop,
                MultBop,
            } bop_id;
            ASTNode* lhs;
            ASTNode* rhs;
        } binop;
        int64_t num_literal;
        char *var_name;
        struct {
            char *func_name;
            ASTNode** arguments;
            size_t size;
        } func_call;
    };
} ASTNode;

typedef struct {
    enum {
        VarName,
        Num,
    } value_type;

    union {
        char *var_name;
        int64_t num;
    };
} CalculactedValue;

typedef struct{
    char* var_name;
    int64_t var_value;
} VarPair;

typedef struct {
    VarPair var_map[STACK_SIZE];
    CalculactedValue value;
} Context;

void InitVarMap(VarPair *var_map, int len);

int64_t FindValue(VarPair *var_map, int len, char *key);

int AddValue(VarPair *var_map, int len, char* name);

void AssignValue(VarPair *var_map, int len, char *key, int64_t value);

void EvalFunctionCall(Context *ctx, ASTNode *node);

void ConvertVarNameToNum(Context *ctx, CalculactedValue *calculacted_value);

void Eval(Context *ctx, ASTNode *node);

ASTNode* CreateBinOpNode(int bop_id, ASTNode *lhs, ASTNode *rhs);

ASTNode* CreateNumLiteralNode(int64_t num_literal);

ASTNode* CreateVarDeclNode(char *var_name);

ASTNode* CreateFuncCallNode(char *func_name, ASTNode **args, size_t sz);

ASTNode* CreateVariableNode(char *name);
