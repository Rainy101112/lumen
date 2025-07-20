/*
 *
 *		parser.c
 *		LUMEN LANGUAGE PROJECT
 *		Rainy101112 - 2025/7/20
 *
 */

#include <token.h>
#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ast.h>

typedef struct {
	const char *source;
	int index;
	token_t *current_token;
} parser_state;

static void next_token(parser_state *state) {
	if (state->current_token) {
		free(state->current_token->text);
		free(state->current_token);
	}
	state->current_token = get_next_token(state->source, &state->index);
}

static ast_node *parse_expression(parser_state *state);
static ast_node *parse_statement(parser_state *state);
static ast_node *parse_block(parser_state *state);

static ast_node *create_ast_node(ast_node_type type) {
	ast_node *node = malloc(sizeof(ast_node));
	if (!node) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	node->type = type;
	return node;
}

static ast_node *parse_primary(parser_state *state) {
	if (!state->current_token) return NULL;

	token_t *tok = state->current_token;
	ast_node *node = NULL;
	
	switch (tok->type) {
		case TOKEN_IDENTIFIER:
			node = create_ast_node(AST_VARIABLE);
			node->data.variable = strdup(tok->text);
			next_token(state);
			break;
			
		case TOKEN_NUMBER:
			node = create_ast_node(AST_LITERAL);
			node->data.literal = atof(tok->text);
			next_token(state);
			break;
			
		case TOKEN_OPEN_PAREN: {
			next_token(state);  // Jump over '('
			node = parse_expression(state);
			if (!node) return NULL;
			
			if (state->current_token && state->current_token->type == TOKEN_CLOSE_PAREN) {
				next_token(state);  // Jump over ')'
			} else {
				fprintf(stderr, "Expected ')' after expression\n");
				free_ast(node);
				return NULL;
			}
			break;
		}
			
		default:
			fprintf(stderr, "Unexpected token: %s\n", tok->text);
			return NULL;
	}
	return node;
}

static ast_node *parse_binary_op(parser_state *state, ast_node *left, int min_prec) {
	while (state->current_token) {
		token_type_t op = state->current_token->type;
		int prec = 0;
		
		// Define operators priority
		if (op == TOKEN_OPERATOR_PLUS || op == TOKEN_OPERATOR_MINUS) prec = 1;
		else if (op == TOKEN_OPERATOR_MULTIPLY || op == TOKEN_OPERATOR_DIVIDE) prec = 2;
		else break;
		
		if (prec < min_prec) break;
		
		token_type_t current_op = op;
		next_token(state);  // Consume operators
		
		ast_node *right = parse_primary(state);
		if (!right) return NULL;
		
		// Handles higher priority operators
		while (state->current_token) {
			token_type_t next_op = state->current_token->type;
			int next_prec = 0;
			
			if (next_op == TOKEN_OPERATOR_PLUS || next_op == TOKEN_OPERATOR_MINUS) next_prec = 1;
			else if (next_op == TOKEN_OPERATOR_MULTIPLY || next_op == TOKEN_OPERATOR_DIVIDE) next_prec = 2;
			else break;
			
			if (next_prec <= prec) break;
			
			right = parse_binary_op(state, right, prec + 1);
			if (!right) return NULL;
		}
		
		ast_node *new_node = create_ast_node(AST_BINARY_OP);
		new_node->data.binop.op = current_op;
		new_node->data.binop.left = left;
		new_node->data.binop.right = right;
		left = new_node;
	}
	return left;
}

static ast_node *parse_expression(parser_state *state) {
	ast_node *left = parse_primary(state);
	if (!left) return NULL;
	
	return parse_binary_op(state, left, 0);
}

static ast_node *parse_assignment(parser_state *state) {
	if (!state->current_token || state->current_token->type != TOKEN_IDENTIFIER) {
		return NULL;
	}
	
	char *var_name = strdup(state->current_token->text);
	next_token(state);  // 消耗标识符
	
	if (!state->current_token || state->current_token->type != TOKEN_ASSIGN) {
		free(var_name);
		return NULL;
	}
	
	next_token(state);  // 消耗 '='
	ast_node *expr = parse_expression(state);
	if (!expr) {
		free(var_name);
		return NULL;
	}
	
	ast_node *node = create_ast_node(AST_ASSIGNMENT);
	node->data.assign.name = var_name;
	node->data.assign.value = expr;
	return node;
}

static ast_node *parse_control_structure(parser_state *state) {
	token_type_t keyword = state->current_token->type;
	next_token(state);  // 消耗关键字
	
	ast_node *node = NULL;
	switch (keyword) {
		case TOKEN_KEYWORD_IF: {
			ast_node *cond = parse_expression(state);
			if (!cond) return NULL;
			
			ast_node *then_block = parse_block(state);
			if (!then_block) {
				free_ast(cond);
				return NULL;
			}
			
			ast_node *else_block = NULL;
			if (state->current_token && state->current_token->type == TOKEN_KEYWORD_ELSE) {
				next_token(state);  // 消耗 else
				else_block = parse_block(state);
			}
			
			node = create_ast_node(AST_IF_STMT);
			node->data.if_stmt.condition = cond;
			node->data.if_stmt.then_block = then_block;
			node->data.if_stmt.else_block = else_block;
			break;
		}
			
		case TOKEN_KEYWORD_FOR: {
			// for(init; cond; update) body
			if (!state->current_token || state->current_token->type != TOKEN_OPEN_PAREN) {
				fprintf(stderr, "Expected '(' after for\n");
				return NULL;
			}
			next_token(state);  // Consume '('
			
			ast_node *init = parse_statement(state);
			ast_node *cond = parse_expression(state);
			
			// Consume semicolon
			if (state->current_token && state->current_token->type == TOKEN_SEMICOLON) {
				next_token(state);
			}
			
			ast_node *update = parse_expression(state);
			
			if (!state->current_token || state->current_token->type != TOKEN_CLOSE_PAREN) {
				fprintf(stderr, "Expected ')' after for conditions\n");
				if (init) free_ast(init);
				if (cond) free_ast(cond);
				if (update) free_ast(update);
				return NULL;
			}
			next_token(state);  // Consume ')'
			
			ast_node *body = parse_block(state);
			if (!body) {
				if (init) free_ast(init);
				if (cond) free_ast(cond);
				if (update) free_ast(update);
				return NULL;
			}
			
			node = create_ast_node(AST_FOR_LOOP);
			node->data.for_loop.init = init;
			node->data.for_loop.condition = cond;
			node->data.for_loop.update = update;
			node->data.for_loop.body = body;
			break;
		}
			
		case TOKEN_KEYWORD_WHILE: {
			ast_node *cond = parse_expression(state);
			if (!cond) return NULL;
			
			ast_node *body = parse_block(state);
			if (!body) {
				free_ast(cond);
				return NULL;
			}
			
			node = create_ast_node(AST_WHILE_LOOP);
			node->data.while_loop.condition = cond;
			node->data.while_loop.body = body;
			break;
		}
			
		case TOKEN_KEYWORD_BREAK:
			node = create_ast_node(AST_BREAK);
			next_token(state);
			break;
			
		case TOKEN_KEYWORD_CONTINUE:
			node = create_ast_node(AST_CONTINUE);
			next_token(state);
			break;
			
		default:
			fprintf(stderr, "Unexpected control keyword\n");
			return NULL;
	}
	return node;
}

static ast_node *parse_statement(parser_state *state) {
	if (!state->current_token) return NULL;
	
	switch (state->current_token->type) {
		case TOKEN_KEYWORD_IF:
		case TOKEN_KEYWORD_FOR:
		case TOKEN_KEYWORD_WHILE:
		case TOKEN_KEYWORD_BREAK:
		case TOKEN_KEYWORD_CONTINUE:
			return parse_control_structure(state);
			
		case TOKEN_IDENTIFIER:
			return parse_assignment(state);
			
		case TOKEN_OPEN_PAREN:
			return parse_block(state);
			
		default:
			return parse_expression(state);
	}
}

static ast_node *parse_block(parser_state *state) {
	if (!state->current_token || state->current_token->type != TOKEN_OPEN_PAREN) {
		fprintf(stderr, "Expected '{' at block start\n");
		return NULL;
	}
	next_token(state);  // Consume '{'
	
	ast_node *node = create_ast_node(AST_BLOCK);
	block_statement block = {0};
	
	while (state->current_token && state->current_token->type != TOKEN_CLOSE_PAREN) {
		ast_node *stmt = parse_statement(state);
		if (!stmt) {
			// Jump over till get '}'
			while (state->current_token && state->current_token->type != TOKEN_CLOSE_PAREN) {
				next_token(state);
			}
			break;
		}
		
		// Add to list
		block.count++;
		block.statements = realloc(block.statements, block.count * sizeof(ast_node*));
		block.statements[block.count - 1] = stmt;
	}
	
	if (state->current_token && state->current_token->type == TOKEN_CLOSE_PAREN) {
		next_token(state);  // Consume '}'
	} else {
		fprintf(stderr, "Expected '}' at block end\n");
	}
	
	node->data.block = block;
	return node;
}

ast_node *parse(const char *source_code) {
	parser_state state = {
		.source = source_code,
		.index = 0,
		.current_token = NULL
	};
	
	next_token(&state);
	ast_node *program = parse_block(&state);
	
	if (state.current_token) {
		free(state.current_token->text);
		free(state.current_token);
	}
	
	return program;
}

void free_ast(ast_node *node) {
	if (!node) return;
	
	switch (node->type) {
		case AST_BLOCK:
			for (int i = 0; i < node->data.block.count; i++) {
				free_ast(node->data.block.statements[i]);
			}
			free(node->data.block.statements);
			break;
			
		case AST_ASSIGNMENT:
			free(node->data.assign.name);
			free_ast(node->data.assign.value);
			break;
			
		case AST_BINARY_OP:
			free_ast(node->data.binop.left);
			free_ast(node->data.binop.right);
			break;
			
		case AST_VARIABLE:
			free(node->data.variable);
			break;
			
		case AST_IF_STMT:
			free_ast(node->data.if_stmt.condition);
			free_ast(node->data.if_stmt.then_block);
			free_ast(node->data.if_stmt.else_block);
			break;
			
		case AST_FOR_LOOP:
			free_ast(node->data.for_loop.init);
			free_ast(node->data.for_loop.condition);
			free_ast(node->data.for_loop.update);
			free_ast(node->data.for_loop.body);
			break;
			
		case AST_WHILE_LOOP:
			free_ast(node->data.while_loop.condition);
			free_ast(node->data.while_loop.body);
			break;
			
		default:
			break;
	}
	
	free(node);
}
