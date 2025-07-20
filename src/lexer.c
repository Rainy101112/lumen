/*
 *
 *		lexer.c
 *		LUMEN LANGUAGE PROJECT
 *		Rainy101112 - 2025/7/20
 *
 */

#include <token.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static token_type_t check_keyword(char *text);

static token_type_t check_keyword(char *text) {
	static const struct
	{
		const char *text;
		token_type_t type;
	} keywords[] = {
		{"if", TOKEN_KEYWORD_IF},
		{"else", TOKEN_KEYWORD_ELSE},
		{"for", TOKEN_KEYWORD_FOR},
		{"while", TOKEN_KEYWORD_WHILE},
		{"break", TOKEN_KEYWORD_BREAK},
		{"continue", TOKEN_KEYWORD_CONTINUE}
	};

	for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
		if (strcmp(text, keywords[i].text) == 0) {
			return keywords[i].type;
		}
	}

	return TOKEN_IDENTIFIER;
}

token_t *get_next_token(const char *source_code, int *index) {
	while (source_code[*index] != '\0') {
		if (isspace(source_code[*index])) {
			(*index)++;
			continue;
		}

		if (isdigit(source_code[*index])) {
			int start_index = *index;

			while (isdigit(source_code[*index])) (*index)++;

			token_t *token = (token_t *)malloc(sizeof(token_t));
			if (!token){
				printf("Memory allocate failed at %s:%d", __FILE__, __LINE__);
			}

			token->type = TOKEN_NUMBER;
			token->text = strndup(source_code + start_index, *index - start_index);
			if (!token->text){
				free(token);
				printf("Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			return token;
		}

		if (isalpha(source_code[*index]) || source_code[*index] == '_') {
			int start_index = *index;

			(*index)++;

			while (isalnum(source_code[*index]) || source_code[*index] == '_') {
				(*index)++;
			}

			char *text = strndup(source_code + start_index, *index - start_index);

			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->type = check_keyword(text);
			token->text = text;

			return token;
		}

		if (source_code[*index] == '+') {
			if (source_code[*index + 1] != '\0' && isdigit(source_code[*index + 1])) {
				int start_index = *index;
				(*index)++;

				while (isdigit(source_code[*index])) (*index)++;

				token_t *token = malloc(sizeof(token_t));
				if (!token) {
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					return NULL;
				}

				token->type = TOKEN_NUMBER;
				token->text = strndup(source_code + start_index, *index - start_index);
				if (!token->text) {
					free(token);
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					return NULL;
				}
				return token;
			} 
			else {
				char *text = strndup(source_code + *index, 1);
				if (!text) {
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					return NULL;
				}

				token_t *token = malloc(sizeof(token_t));
				if (!token) {
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					free(text);
					return NULL;
				}

				token->text = text;
				token->type = TOKEN_OPERATOR_MINUS;

				(*index)++;
				return token;
			}
		}

		if (source_code[*index] == '-') {
			if (source_code[*index + 1] != '\0' && isdigit(source_code[*index + 1])) {
				int start_index = *index;
				(*index)++;

				while (isdigit(source_code[*index])) (*index)++;

				token_t *token = malloc(sizeof(token_t));
				if (!token) {
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					return NULL;
				}

				token->type = TOKEN_NUMBER;
				token->text = strndup(source_code + start_index, *index - start_index);
				if (!token->text) {
					free(token);
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					return NULL;
				}
				return token;
			} 
			else {
				char *text = strndup(source_code + *index, 1);
				if (!text) {
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					return NULL;
				}

				token_t *token = malloc(sizeof(token_t));
				if (!token) {
					fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
					free(text);
					return NULL;
				}

				token->text = text;
				token->type = TOKEN_OPERATOR_MINUS;

				(*index)++;
				return token;
			}
		}

		if (source_code[*index] == '*') {
			char *text = strndup(source_code + *index, 1);
			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->text = text;
			token->type = TOKEN_OPERATOR_MULTIPLY;

			(*index)++;
			return token;
		}

		if (source_code[*index] == '/') {
			char *text = strndup(source_code + *index, 1);
			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->text = text;
			token->type = TOKEN_OPERATOR_DIVIDE;

			(*index)++;
			return token;
		}

		if (source_code[*index] == ';') {
			char *text = strndup(source_code + *index, 1);
			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->text = text;
			token->type = TOKEN_SEMICOLON;

			(*index)++;
			return token;
		}

		if (source_code[*index] == '=') {
			char *text = strndup(source_code + *index, 1);
			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->text = text;
			token->type = TOKEN_ASSIGN;

			(*index)++;
			return token;
		}

		if (source_code[*index] == '{') {
			char *text = strndup(source_code + *index, 1);
			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->text = text;
			token->type = TOKEN_OPEN_PAREN;

			(*index)++;
			return token;
		}

		if (source_code[*index] == '}') {
			char *text = strndup(source_code + *index, 1);
			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->text = text;
			token->type = TOKEN_CLOSE_PAREN;

			(*index)++;
			return token;
		}


		if (source_code[*index] == (char)EOF) {
			char *text = strndup(source_code + *index, 1);
			if (!text) {
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				return NULL;
			}

			token_t *token = malloc(sizeof(token_t));
			if (!token){
				fprintf(stderr, "Memory allocate failed at %s:%d", __FILE__, __LINE__);
				free(text);
				return NULL;
			}

			token->text = text;
			token->type = TOKEN_END_OF_FILE;

			(*index)++;
			return token;
		}

		fprintf(stderr, "Unrecognized character: %c\n", source_code[*index]);

		(*index)++;
		continue;
	}

	return NULL;
}
