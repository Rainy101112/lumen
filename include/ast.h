/*
 *
 *		ast.h
 *		LUMEN LANGUAGE PROJECT
 *		Rainy101112 - 2025/7/20
 *
 */

#pragma once

#include "token.h"

typedef enum {
	AST_PROGRAM,
	AST_BLOCK,
	AST_ASSIGNMENT,
	AST_BINARY_OP,
	AST_VARIABLE,
	AST_LITERAL,
	AST_IF_STMT,
	AST_FOR_LOOP,
	AST_WHILE_LOOP,
	AST_BREAK,
	AST_CONTINUE
} ast_node_type;

typedef struct ast_node ast_node;

typedef struct {
	ast_node **statements;
	int count;
} block_statement;

typedef struct {
	char *name;
	ast_node *value;
} assignment;

typedef struct {
	token_type_t op;
	ast_node *left;
	ast_node *right;
} binary_operation;

typedef struct {
	ast_node *condition;
	ast_node *then_block;
	ast_node *else_block;
} if_statement;

typedef struct {
	ast_node *init;
	ast_node *condition;
	ast_node *update;
	ast_node *body;
} for_loop;

typedef struct {
	ast_node *condition;
	ast_node *body;
} while_loop;

struct ast_node {
	ast_node_type type;
	union {
		block_statement block;
		assignment assign;
		binary_operation binop;
		char *variable;
		double literal;
		if_statement if_stmt;
		for_loop for_loop;
		while_loop while_loop;
	} data;
};

ast_node *parse(const char *source_code);
void free_ast(ast_node *node);

