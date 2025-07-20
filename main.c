#include <stdio.h>
#include <stdlib.h>
#include <token.h>
#include <lexer.h>
#include <lumen.h>
#include <time.h>

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


	clock_end = clock();
	time_total = (double)((clock_end - clock_start) / CLOCKS_PER_SEC);
	printf(LOG_LEVEL_LOGGER "Program stopped. Time used: %.5f second(s).\n", time_total);

	return 0;
}