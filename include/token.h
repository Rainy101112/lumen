#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum token_type {
	TOKEN_IDENTIFIER,
	TOKEN_NUMBER,

	TOKEN_KEYWORD_IF,
	TOKEN_KEYWORD_ELSE,
	TOKEN_KEYWORD_BREAK,
	TOKEN_KEYWORD_CONTINUE,

	TOKEN_OPERATOR_PLUS,
	TOKEN_OPERATOR_MINUS,
	TOKEN_OPERATOR_MULTIPLY,
	TOKEN_OPERATOR_DIVIDE,

	TOKEN_ASSIGN,

	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,

	TOKEN_END_OF_FILE
} token_type_t;

typedef struct token {
	token_type_t type;
	char *text;
} token_t;
