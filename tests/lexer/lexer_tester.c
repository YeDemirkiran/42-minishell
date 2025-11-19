/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:56:01 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/19 22:29:29 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <modules/lexer.h>
#include <libft/string.h>

#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_WHITE "\e[0;37m"

static size_t	get_tokens_count(t_token *tokens)
{
	size_t	i;

	i = 0;
	while (tokens[i].type != TOKEN_EOF)
		i++;
	return (i);
}

void	test_lexical_analyze(int test_number)
{
	t_token	*tokens;
	int		bool;
	size_t	size;

	printf("\nTest %i: lexical_analyze()\n", test_number);

	// Test 1
	printf("\n%i.1: NULL Input\n", test_number);
	printf("Calling lexical_analyze(NULL)...\n");
	printf("================================\n");
	tokens = lexical_analyze(NULL);
	printf("================================\n");
	bool = tokens == NULL;
	printf("%s[%s] " COLOR_WHITE "Tokens NULL: %s, Expected: YES\n", bool ? COLOR_GREEN : COLOR_RED, bool ? "OK" : "KO", bool ? "YES" : "NO");

	// Test 2
	printf("\n%i.2: Empty Input\n", test_number);
	printf("Calling lexical_analyze(\"\")...\n");
	printf("================================\n");
	tokens = lexical_analyze("");
	printf("================================\n");
	size = get_tokens_count(tokens);
	bool = size == 0;
	printf("%s[%s] " COLOR_WHITE "Tokens size: %lu, Expected: 0\n", bool ? COLOR_GREEN : COLOR_RED, bool ? "OK" : "KO", size);
	free(tokens);

	// Test 3
	printf("\n%i.3: Single Word\n", test_number);
	printf("Calling lexical_analyze(\"word\")...\n");
	printf("================================\n");
	tokens = lexical_analyze("word");
	printf("================================\n");
	size = get_tokens_count(tokens);
	bool = ft_strncmp(tokens[0].data, "word", 4) == 0;
	printf("%s[%s] " COLOR_WHITE "Tokens Info: {size: %lu, first_token: %s}\n\tExpected: {size: 1, first_token: word}\n", bool ? COLOR_GREEN : COLOR_RED, bool ? "OK" : "KO", size, tokens[0].data);
	free(tokens);

	printf("\n[DONE: Test %i: lexical_analyze()]\n", test_number);
}

int	main(void)
{
	printf("\nLEXER TESTER\n");

	// Test 1
	test_lexical_analyze(1);

	return (EXIT_SUCCESS);
}
