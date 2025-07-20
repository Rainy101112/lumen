/*
 *
 *		lexer.h
 *		LUMEN LANGUAGE PROJECT
 *		Rainy101112 - 2025/7/20
 *
 */

#pragma once

#include <token.h>

token_t *get_next_token(const char *source_code, int *index);
