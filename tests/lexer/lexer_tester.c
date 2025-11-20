/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:56:01 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/20 13:07:19 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <modules/lexer.h>
#include <libft/string.h>

#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_WHITE "\e[0;37m"

typedef struct s_lexical_test
{
    char	*input;
	t_token		*retrieved_tokens;
	t_token		*expected_tokens;
} 	t_lexical_test;

static size_t	get_tokens_count(t_token *tokens)
{
	size_t	i;

	if (tokens == NULL)
	return (0);
	i = 0;
	while (tokens[i].type != TOKEN_EOF)
		i++;
	return (i);
}

static int	tests_match(t_lexical_test test)
{
	if ((test.expected_tokens == NULL) != (test.retrieved_tokens == NULL))
		return (0);
	if (get_tokens_count(test.expected_tokens) != get_tokens_count(test.retrieved_tokens))
		return (0);
	if (test.retrieved_tokens == NULL && test.expected_tokens == NULL)
		return (1);
	for (size_t i = 0; test.retrieved_tokens[i].type != TOKEN_EOF; i++)
	{
		if (test.retrieved_tokens[i].type != test.expected_tokens[i].type)
			return (0);
		if (test.retrieved_tokens[i].type == TOKEN_STRING && ft_strncmp(test.retrieved_tokens[i].data, test.expected_tokens[i].data, ft_strlen(test.retrieved_tokens[i].data)))
			return (0);
	}

	return (1);
}

static void	debug_print_tokens(t_token *tokens)
{
	int	i;

	if (tokens == NULL)
	{
		printf("(NULL)\n");
		return ;
	}
	i = 0;
	printf("(");
	while (tokens[i].type != TOKEN_EOF)
	{
		if (tokens[i].type == TOKEN_BLANK)
			printf("[BLANK]");
		else if (tokens[i].type == TOKEN_REDIR)
			printf("[REDIR]");
		else if (tokens[i].type == TOKEN_PIPE)
			printf("[PIPE]");
		else if (tokens[i].type == TOKEN_PARENTHESIS)
			printf("[PAREN]");
		else if (tokens[i].type == TOKEN_AMPERSAND)
			printf("[AMP]");
		else if (tokens[i].type == TOKEN_NEWLINE)
			printf("[NEWLINE]");
		else
			printf("%s", tokens[i].data);
		if (tokens[i + 1].type != TOKEN_EOF)
			printf(", ");
		fflush(stdout);
		i++;
	}
	if (i == 0)
		printf("none");
	printf(")\n");
}

static void	conduct_test(t_lexical_test test)
{
	int	result;

	printf("Calling lexical_analyze(%c%s%c)...\n", test.input == NULL ? 0 : '"', test.input == NULL ? "NULL" : test.input, test.input == NULL ? 0 : '"');
	printf("=============================================================\n");
	test.retrieved_tokens = lexical_analyze(test.input);
	printf("=============================================================\n");

	result = tests_match(test) == 1;
	printf("[EXPECTED]  NULL: %s, Size: %lu, Tokens: ", test.expected_tokens == NULL ? "Yes" : "No", get_tokens_count(test.expected_tokens));
	debug_print_tokens(test.expected_tokens);
	printf("[RETRIEVED] NULL: %s, Size: %lu, Tokens: ", test.retrieved_tokens == NULL ? "Yes" : "No", get_tokens_count(test.retrieved_tokens));
	debug_print_tokens(test.retrieved_tokens);
	printf("%s[%s]" COLOR_WHITE"\n", result ? COLOR_GREEN : COLOR_RED, result ? "OK" : "KO");

	free(test.retrieved_tokens);
}

void	test_lexical_analyze(int test_number)
{
	printf("\nTest %i: lexical_analyze()\n", test_number);

	printf("Test %i.1: Basic Words & Spaces\n", test_number);

	// Test 1.1
	printf("\n%i.1.1: NULL Input\n", test_number);
	conduct_test((t_lexical_test){
		.input = NULL,
		.retrieved_tokens = NULL,
		.expected_tokens = NULL
	});

	// Test 1.2
	printf("\n%i.1.2: Empty Input\n", test_number);
	conduct_test((t_lexical_test){
		.input = "",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 1.3
	printf("\n%i.1.3: Single Word\n", test_number);
	conduct_test((t_lexical_test){
		.input = "word1",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "word1"},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 1.4
	printf("\n%i.1.4: Two Words\n", test_number);
	conduct_test((t_lexical_test){
		.input = "word1 word2",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "word1"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word2"},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 1.5
	printf("\n%i.1.5: Multiple Words\n", test_number);
	conduct_test((t_lexical_test){
		.input = "word1 word2 word3 word4 word5 word6 word7 word8 word9 word10",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "word1"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word2"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word3"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word4"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word5"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word6"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word7"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word8"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word9"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word10"},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 1.6
	printf("\n%i.1.6: Single Space\n", test_number);
	conduct_test((t_lexical_test){
		.input = " ",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 1.7
	printf("\n%i.1.7: Multiple Spaces\n", test_number);
	conduct_test((t_lexical_test){
		.input = "                    ",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 1.8
	printf("\n%i.1.8: Single Word Wrapped Between Multiple Spaces\n", test_number);
	conduct_test((t_lexical_test){
		.input = "          word          ",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 1.9
	printf("\n%i.1.9: Multiple Words Wrapped Between Spaces\n", test_number);
	conduct_test((t_lexical_test){
		.input = "  word1  word2  word3  word4  word5  ",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word1"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word2"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word3"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word4"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_STRING, .data = "word5"},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_BLANK, .data = NULL},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	printf("Test %i.2: Single & Double Quotes\n", test_number);

	// Test 2.1
	printf("\n%i.2.1: One Single Quote\n", test_number);
	conduct_test((t_lexical_test){
		.input = "'",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "'"},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 2.2
	printf("\n%i.2.2: One Double Quote\n", test_number);
	conduct_test((t_lexical_test){
		.input = "\"",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "\""},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 2.3
	printf("\n%i.2.3: Empty Single Quotes\n", test_number);
	conduct_test((t_lexical_test){
		.input = "''",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "''"},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 2.4
	printf("\n%i.2.4: Empty Double Quotes\n", test_number);
	conduct_test((t_lexical_test){
		.input = "\"\"",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "\"\""},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 2.5
	printf("\n%i.2.5: Single Quotes With Only Spaces\n", test_number);
	conduct_test((t_lexical_test){
		.input = "'          '",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "'          '"},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	// Test 2.6
	printf("\n%i.2.6: Double Quotes With Only Spaces\n", test_number);
	conduct_test((t_lexical_test){
		.input = "\"          \"",
		.retrieved_tokens = NULL,
		.expected_tokens = (t_token[]){
			{.type = TOKEN_STRING, .data = "\"          \""},
			{.type = TOKEN_EOF, .data = NULL}
		}
	});

	printf("\n[DONE: Test %i: lexical_analyze()]\n", test_number);
}

int	main(void)
{
	printf("\nLEXER TESTER\n");

	// Test 1
	test_lexical_analyze(1);

	return (EXIT_SUCCESS);
}
