/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:56:01 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/19 21:21:30 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <modules/lexer.h>

# define COLOR_GREEN "\e[0;32m"
# define COLOR_RED "\e[0;31m"
# define COLOR_WHITE "\e[0;37m"

void	test_lexical_analyze(int test_number)
{
	t_token	*tokens;
	int		bool;

	printf("\nTest %i: lexical_analyze()\n", test_number);

	printf("\n%i.1: NULL Input\n", test_number);
	printf("Calling lexical_analyze(NULL)...\n");
	printf("================================\n");
	tokens = lexical_analyze(NULL);
	printf("================================\n");
	bool = tokens == NULL;
	printf("%s[%s] " COLOR_WHITE "Tokens NULL: %s, Expected: YES\n", bool ? COLOR_GREEN : COLOR_RED, bool ? "OK" : "KO", bool ? "YES" : "NO");
}

int	main(void)
{
	printf("\nLEXER TESTER\n");

	// Test 1
	test_lexical_analyze(1);

	return (EXIT_SUCCESS);
}
