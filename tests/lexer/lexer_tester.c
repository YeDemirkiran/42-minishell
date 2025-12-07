/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:56:01 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/07 20:55:49 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <modules/lexer.h>
#include <lexer_utils.h>

#include <libft/string.h>
#include <libft/linked_list.h>

#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_WHITE "\e[0;37m"

typedef struct s_lexical_test
{
    char	*input;
	t_token	*retrieved_tokens;
	t_token	*expected_tokens;
	size_t	retrieved_length;
	size_t	expected_length;
} 	t_lexical_test;

typedef struct s_test_result
{
	int	total;
	int	passed;
	int	failed;
}	t_test_result;

static t_token	*linked_list_to_token_array(t_list *lst, size_t *length_ref)
{
	size_t	length;
	size_t	i;
	t_token	*tokens;

	if (lst == NULL)
		return (NULL);
	length = ft_lstsize(lst);
	if (length == 0)
		return (NULL);
	tokens = malloc(sizeof(t_token) * length);
	if (tokens == NULL)
		return (NULL);
	i = 0;
	while (lst != NULL)
	{
		tokens[i] = *(t_token*)(lst->content);
		lst = lst->next;
		i++;
	}
	*length_ref = length;
	return (tokens);
}

static t_bool	is_token_redir(t_token_type type)
{
	if (type == TOKEN_REDIR_INPUT || type == TOKEN_REDIR_OUTPUT
		|| type == TOKEN_REDIR_HEREDOC || type == TOKEN_REDIR_APPEND)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

static int	tests_match(t_lexical_test test)
{
	if ((test.expected_tokens == NULL) != (test.retrieved_tokens == NULL))
		return (0);
	if (test.retrieved_length != test.expected_length)
		return (0);
	if (test.retrieved_tokens == NULL && test.expected_tokens == NULL)
		return (1);
	for (size_t i = 0; i < test.expected_length; i++)
	{
		if (test.retrieved_tokens[i].type != test.expected_tokens[i].type)
			return (0);
		if (test.retrieved_tokens[i].type == TOKEN_WORD && ft_strncmp(test.retrieved_tokens[i].data.word, test.expected_tokens[i].data.word, ft_strlen(test.retrieved_tokens[i].data.word)))
			return (0);
		else if (is_token_redir(test.retrieved_tokens[i].type) && test.retrieved_tokens[i].data.number != test.expected_tokens[i].data.number)
			return (0);
	}
	return (1);
}

static void	debug_print_tokens(t_token *tokens, size_t length)
{
	t_token	*token;
	size_t	i;

	i = 0;
	while (i < length)
	{
		token = &(tokens[i]);
		if (token->type == TOKEN_REDIR_INPUT)
			printf("[REDIR_INPUT, FD: %i]", token->data.number);
		else if (token->type == TOKEN_REDIR_OUTPUT)
			printf("[REDIR_OUTPUT, FD: %i]", token->data.number);
		else if (token->type == TOKEN_REDIR_HEREDOC)
			printf("[REDIR_HEREDOC, FD: %i]", token->data.number);
		else if (token->type == TOKEN_REDIR_APPEND)
			printf("[REDIR_APPEND, FD: %i]", token->data.number);
		else if (token->type == TOKEN_PIPE)
			printf("[PIPE]");
		else if (token->type == TOKEN_PAREN_LEFT)
			printf("[PAREN_LEFT]");
		else if (token->type == TOKEN_PAREN_RIGHT)
			printf("[PAREN_RIGHT]");
		else if (token->type == TOKEN_IF_AND)
			printf("[IF_AND]");
		else if (token->type == TOKEN_IF_OR)
			printf("[IF_OR]");
		else if (token->type == TOKEN_NEWLINE)
			printf("[NEWLINE]");
		else if (token->type == TOKEN_WORD)
			printf("[TOKEN_WORD, %s]", token->data.word);
		if (i != length)
			printf(", ");
	}
}


static void	conduct_lexical_analysis_test(t_lexical_test test, t_test_result *current_metrics)
{
	int	result;
	static int	passed;
	static int	failed;

	if (current_metrics != NULL)
	{
		current_metrics->passed = passed;
		current_metrics->failed = failed;
		current_metrics->total = passed + failed;
		return ;
	}

	printf("Calling lexical_analyze(%c%s%c)...\n", test.input == NULL ? 0 : '"', test.input == NULL ? "NULL" : test.input, test.input == NULL ? 0 : '"');
	printf("=============================================================\n");
	test.retrieved_tokens = linked_list_to_token_array(lexical_analyze(test.input), &(test.retrieved_length));
	printf("=============================================================\n");

	result = tests_match(test) == 1;
	passed += result == 1;
	failed += result == 0;

	printf("[EXPECTED]  NULL: %s, Size: %lu, Tokens: ", test.expected_tokens == NULL ? "Yes" : "No", test.expected_length);
	debug_print_tokens(test.expected_tokens, test.expected_length);
	printf("[RETRIEVED] NULL: %s, Size: %lu, Tokens: ", test.retrieved_tokens == NULL ? "Yes" : "No", test.retrieved_length);
	debug_print_tokens(test.retrieved_tokens, test.retrieved_length);
	printf("%s[%s]" COLOR_WHITE"\n", result ? COLOR_GREEN : COLOR_RED, result ? "OK" : "KO");

	free(test.retrieved_tokens);
}

void	test_lexical_analyze(int test_number)
{
	t_lexical_test tests[] = {
		{
			.input = NULL,
			.expected_tokens = NULL,
			.expected_length = 0
		},
		{
			.input = "word1",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = NULL}
			},
			.expected_length = 1
		},
	};

	(void)tests;

	printf("\nTest %i: lexical_analyze()\n", test_number);

	printf("Test %i.1: Basic Words & Spaces\n", test_number);

	// Test 1.1
	printf("\n%i.1.1: NULL Input\n", test_number);
	conduct_lexical_analysis_test((t_lexical_test){
		.input = NULL,
		.expected_tokens = NULL,
		.expected_length = 0,
	}, NULL);

	// Test 1.2
	printf("\n%i.1.2: Empty Input\n", test_number);
	conduct_lexical_analysis_test((t_lexical_test){
		.input = "",
		.expected_tokens = NULL,
		.expected_length = 0,
	}, NULL);

	// // Test 1.3
	// printf("\n%i.1.3: Single Word\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.expected_tokens = (t_token[]){
	// 		{.type = 0, .data = "word1"},
	// 	}

	// }, NULL);

	// // Test 1.4
	// printf("\n%i.1.4: Two Words\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "word1 word2",
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "word1"},
	// 		{.type = TOKEN_WORD, .data = "word2"}
	// 	},
	// 	.expected_length = 2
	// }, NULL);

	// // Test 1.5
	// printf("\n%i.1.5: Multiple Words\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "word1 word2 word3 word4 word5 word6 word7 word8 word9 word10",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "word1"},
	// 		{.type = TOKEN_WORD, .data = "word2"},
	// 		{.type = TOKEN_WORD, .data = "word3"},
	// 		{.type = TOKEN_WORD, .data = "word4"},
	// 		{.type = TOKEN_WORD, .data = "word5"},
	// 		{.type = TOKEN_WORD, .data = "word6"},
	// 		{.type = TOKEN_WORD, .data = "word7"},
	// 		{.type = TOKEN_WORD, .data = "word8"},
	// 		{.type = TOKEN_WORD, .data = "word9"},
	// 		{.type = TOKEN_WORD, .data = "word10"},
	// 	}
	// }, NULL);

	// // Test 1.6
	// printf("\n%i.1.6: Single Space\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = " ",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = NULL
	// }, NULL);

	// // Test 1.7
	// printf("\n%i.1.7: Multiple Spaces\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "                    ",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = NULL
	// }, NULL);

	// // Test 1.8
	// printf("\n%i.1.8: Single Word Wrapped Between Multiple Spaces\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "          word          ",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "word"},
	// 	}
	// }, NULL);

	// // Test 1.9
	// printf("\n%i.1.9: Multiple Words Wrapped Between Spaces\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "  word1  word2  word3  word4  word5  ",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "word1"},
	// 		{.type = TOKEN_WORD, .data = "word2"},
	// 		{.type = TOKEN_WORD, .data = "word3"},
	// 		{.type = TOKEN_WORD, .data = "word4"},
	// 		{.type = TOKEN_WORD, .data = "word5"},
	// 	}
	// }, NULL);

	// printf("Test %i.2: Single & Double Quotes\n", test_number);

	// // Test 2.1
	// printf("\n%i.2.1: One Single Quote\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "'"},
	// 	}
	// }, NULL);

	// // Test 2.2
	// printf("\n%i.2.2: One Double Quote\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\""},
	// 	}
	// }, NULL);

	// // Test 2.3
	// printf("\n%i.2.3: Empty Single Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "''",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "''"},
	// 	}
	// }, NULL);

	// // Test 2.4
	// printf("\n%i.2.4: Empty Double Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"\"",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\"\""},
	// 	}
	// }, NULL);

	// // Test 2.5
	// printf("\n%i.2.5: Single Quotes With Only Spaces\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'          '",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "'          '"},
	// 	}
	// }, NULL);

	// // Test 2.6
	// printf("\n%i.2.6: Double Quotes With Only Spaces\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"          \"",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\"          \""},
	// 	}
	// }, NULL);

	// // Test 2.7
	// printf("\n%i.2.7: Single Quotes Within Double Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"''''''''''''''''''''''''\"",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\"''''''''''''''''''''''''\""},
	// 	}
	// }, NULL);

	// // Test 2.8
	// printf("\n%i.2.8: Double Quotes Within Single Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'\"\"\"\"\"\"\"\"\"\"'",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "'\"\"\"\"\"\"\"\"\"\"'"},
	// 	}
	// }, NULL);

	// // Test 2.9
	// printf("\n%i.2.9: Single Word Within Single Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'word'",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "'word'"},
	// 	}
	// }, NULL);

	// // Test 2.10
	// printf("\n%i.2.10: Single Word Within Double Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"word\"",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\"word\"",},
	// 	}
	// }, NULL);

	// // Test 2.11
	// printf("\n%i.2.11: Multiple Words Within Single Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'word1 word2 word3 word4 word5'",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "'word1 word2 word3 word4 word5'"},
	// 	}
	// }, NULL);

	// // Test 2.12
	// printf("\n%i.2.12: Multiple Words Within Double Quotes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"word1 word2 word3 word4 word5\"",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\"word1 word2 word3 word4 word5\""},
	// 	}
	// }, NULL);

	// // Test 2.13
	// printf("\n%i.2.13: Mixed Qutoes\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "   \"word1 word2 word3 word4 word5\" 'word6 word7' '' \"\" '     ' \"     \"   ",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\"word1 word2 word3 word4 word5\""},
	// 		{.type = TOKEN_WORD, .data = "'word6 word7'"},
	// 		{.type = TOKEN_WORD, .data = "''"},
	// 		{.type = TOKEN_WORD, .data = "\"\""},
	// 		{.type = TOKEN_WORD, .data = "'     '"},
	// 		{.type = TOKEN_WORD, .data = "\"     \""},
	// 	}
	// }, NULL);

	// // Test 2.14
	// printf("\n%i.2.14: Unclosed single quote\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "'"},
	// 	}
	// }, NULL);

	// // Test 2.15
	// printf("\n%i.2.15: Unclosed double quote\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\""},
	// 	}
	// }, NULL);

	// // Test 2.16
	// printf("\n%i.2.16: Unclosed single quote with words\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'word1 word2 word3 word4 word5 word6",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "'word1 word2 word3 word4 word5 word6"},
	// 	}
	// }, NULL);

	// // Test 2.17
	// printf("\n%i.2.17: Unclosed double quote with words\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "\"word1 word2 word3 word4 word5 word6",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\"word1 word2 word3 word4 word5 word6"},
	// 	}
	// }, NULL);

	// // Test 2.18
	// printf("\n%i.2.18: Many Quotes With No Spaces\n", test_number);
	// conduct_lexical_analysis_test((t_lexical_test){
	// 	.input = "'word1'word2'word3'word4\"word5\"word6\"word7\"word8'word9\"word10\"word11'word12",
	// 	.retrieved_tokens = NULL,
	// 	.expected_tokens = (t_token[]){
	// 		{.type = TOKEN_WORD, .data = "\'word1'word2'word3'word4\"word5\"word6\"word7\"word8'word9\"word10\"word11'word12"},
	// 	}
	// }, NULL);

	t_test_result	results;
	conduct_lexical_analysis_test((t_lexical_test){}, &results);

	printf("\n==================[RESULTS]==================\n\n");
	printf(COLOR_GREEN "PASS: " COLOR_WHITE "%i/%i\n", results.passed, results.total);
	printf(COLOR_RED "FAIL: " COLOR_WHITE "%i/%i\n", results.failed, results.passed);
	printf("\n[TEST %i DONE]\n", test_number);
}

int	main(void)
{
	printf("\nLEXER TESTER\n");

	test_lexical_analyze(1);

	return (EXIT_SUCCESS);
}
