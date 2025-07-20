/*
 *
 *		ast_parser.h
 *		LUMEN LANGUAGE PROJECT
 *		Rainy101112 - 2025/7/20
 *
 */

#pragma once

typedef enum {
	AST_ASSIGNMENT,
	AST_IF_STATEMENT,
	AST_WHILE_STATEMENT,
	AST_EXPRESSION,
	AST_TERM,
	AST_FACTOR
} ast_node_t;

typedef struct astnode {
	ast_node_t type;
	struct astnode *left;
	struct astnode *right;
	char *value;
} astnode;
