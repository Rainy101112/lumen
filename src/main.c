/*
 *
 *		main.c
 *		LUMEN LANGUAGE PROJECT
 *		Rainy101112 - 2025/7/20
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <token.h>
#include <lexer.h>
#include <lumen.h>
#include <time.h>
#include <ast.h>

void print_ast(ast_node *node, int indent) {
	if (!node) return;
	
	char indent_str[32] = {0};
	for (int i = 0; i < indent; i++) indent_str[i] = ' ';
	
	switch (node->type) {
		case AST_BLOCK:
			printf(LOG_LEVEL_LOGGER "%sBLOCK:\n", indent_str);
			for (int i = 0; i < node->data.block.count; i++) {
				print_ast(node->data.block.statements[i], indent + 4);
			}
			break;
			
		case AST_ASSIGNMENT:
			printf(LOG_LEVEL_LOGGER "%sASSIGNMENT:\n", indent_str);
			printf(LOG_LEVEL_LOGGER "%s  Variable: %s\n", indent_str, node->data.assign.name);
			printf(LOG_LEVEL_LOGGER "%s  Value:\n", indent_str);
			print_ast(node->data.assign.value, indent + 4);
			break;
			
		case AST_BINARY_OP: {
			const char *op_str = "UNKNOWN";
			switch (node->data.binop.op) {
				case TOKEN_OPERATOR_PLUS: op_str = "+"; break;
				case TOKEN_OPERATOR_MINUS: op_str = "-"; break;
				case TOKEN_OPERATOR_MULTIPLY: op_str = "*"; break;
				case TOKEN_OPERATOR_DIVIDE: op_str = "/"; break;
				default: break;
			}
			printf(LOG_LEVEL_LOGGER "%sBINARY_OP (%s):\n", indent_str, op_str);
			printf(LOG_LEVEL_LOGGER "%s  Left:\n", indent_str);
			print_ast(node->data.binop.left, indent + 4);
			printf(LOG_LEVEL_LOGGER "%s  Right:\n", indent_str);
			print_ast(node->data.binop.right, indent + 4);
			break;
		}
			
		case AST_VARIABLE:
			printf(LOG_LEVEL_LOGGER "%sVARIABLE: %s\n", indent_str, node->data.variable);
			break;
			
		case AST_LITERAL:
			printf(LOG_LEVEL_LOGGER "%sLITERAL: %f\n", indent_str, node->data.literal);
			break;
			
		case AST_IF_STMT:
			printf(LOG_LEVEL_LOGGER "%sIF_STATEMENT:\n", indent_str);
			printf(LOG_LEVEL_LOGGER "%s  Condition:\n", indent_str);
			print_ast(node->data.if_stmt.condition, indent + 4);
			printf(LOG_LEVEL_LOGGER "%s  Then:\n", indent_str);
			print_ast(node->data.if_stmt.then_block, indent + 4);
			if (node->data.if_stmt.else_block) {
				printf(LOG_LEVEL_LOGGER "%s  Else:\n", indent_str);
				print_ast(node->data.if_stmt.else_block, indent + 4);
			}
			break;
			
		case AST_FOR_LOOP:
			printf(LOG_LEVEL_LOGGER "%sFOR_LOOP:\n", indent_str);
			if (node->data.for_loop.init) {
				printf(LOG_LEVEL_LOGGER "%s  Init:\n", indent_str);
				print_ast(node->data.for_loop.init, indent + 4);
			}
			if (node->data.for_loop.condition) {
				printf(LOG_LEVEL_LOGGER "%s  Condition:\n", indent_str);
				print_ast(node->data.for_loop.condition, indent + 4);
			}
			if (node->data.for_loop.update) {
				printf(LOG_LEVEL_LOGGER "%s  Update:\n", indent_str);
				print_ast(node->data.for_loop.update, indent + 4);
			}
			printf(LOG_LEVEL_LOGGER "%s  Body:\n", indent_str);
			print_ast(node->data.for_loop.body, indent + 4);
			break;
			
		case AST_WHILE_LOOP:
			printf(LOG_LEVEL_LOGGER "%sWHILE_LOOP:\n", indent_str);
			printf(LOG_LEVEL_LOGGER "%s  Condition:\n", indent_str);
			print_ast(node->data.while_loop.condition, indent + 4);
			printf(LOG_LEVEL_LOGGER "%s  Body:\n", indent_str);
			print_ast(node->data.while_loop.body, indent + 4);
			break;
			
		case AST_BREAK:
			printf(LOG_LEVEL_LOGGER "%sBREAK\n", indent_str);
			break;
			
		case AST_CONTINUE:
			printf(LOG_LEVEL_LOGGER "%sCONTINUE\n", indent_str);
			break;
			
		default:
			printf(LOG_LEVEL_LOGGER "%sUNKNOWN_NODE_TYPE: %d\n", indent_str, node->type);
			break;
	}
}

int main(int argc, char *argv[]){
	clock_t clock_start, clock_end;
	double time_total;
	clock_start = clock();


	for (int i = 0; i <= argc; i++) printf(LOG_LEVEL_LOGGER "argv[%d] = %s\n", i, argv[i]);


	int index = 0;
	token_t *token = get_next_token("-100", &index);

	printf("TOKEN_TYPE: %d\n", token->type);
	printf("TOKEN_TEXT: %s\n", token->text);

	free(token->text);
	free(token);

	const char *code1 = "{ x = 5 + 3 * 2; }";
	printf(LOG_LEVEL_LOGGER "Parsing Code 1\n%s\n", code1);
	ast_node *ast1 = parse(code1);
	print_ast(ast1, 0);
	free_ast(ast1);
	
	printf("\n\n");
	
	const char *code2 = 
		"{"
		"  a = 10;"
		"  if (a > 5) {"
		"	b = 20;"
		"  } else {"
		"	b = 0;"
		"  }"
		"  for (i = 0; i < 10; i = i + 1) {"
		"	c = c + i;"
		"	if (c > 15) {"
		"	  break;"
		"	}"
		"  }"
		"  while (c < 100) {"
		"	c = c * 2;"
		"  }"
		"}";
	
	printf(LOG_LEVEL_LOGGER "Parsing Code 2\n%s\n", code2);
	ast_node *ast2 = parse(code2);
	print_ast(ast2, 0);
	free_ast(ast2);

	clock_end = clock();
	time_total = (double)((clock_end - clock_start) / CLOCKS_PER_SEC);
	printf(LOG_LEVEL_LOGGER "Program stopped. Time used: %.5f second(s).\n", time_total);

	return 0;
}