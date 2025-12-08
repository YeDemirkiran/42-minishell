/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tester.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 20:56:01 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/08 17:31:39 by yademirk         ###   ########.fr       */
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
		if (i + 1 != length)
			printf(", ");
		i++;
	}
	printf("\n");
}


static t_test_result	conduct_lexical_analysis_test(t_lexical_test test)
{
	static t_test_result	results;
	int						result;

	printf("\nlexical_analyze(%c%s%c)\n", test.input == NULL ? 0 : '"', test.input == NULL ? "NULL" : test.input, test.input == NULL ? 0 : '"');
	printf("=============================================================\n");
	test.retrieved_tokens = linked_list_to_token_array(lexical_analyze(test.input), &(test.retrieved_length));
	printf("=============================================================\n");

	result = tests_match(test) == 1;
	results.passed += result == 1;
	results.failed += result == 0;
	results.total += 1;

	printf("[EXPECTED]  NULL: %s, Size: %lu, Tokens: ", test.expected_tokens == NULL ? "Yes" : "No", test.expected_length);
	debug_print_tokens(test.expected_tokens, test.expected_length);
	printf("[RETRIEVED] NULL: %s, Size: %lu, Tokens: ", test.retrieved_tokens == NULL ? "Yes" : "No", test.retrieved_length);
	debug_print_tokens(test.retrieved_tokens, test.retrieved_length);
	printf("%s[%s]" COLOR_WHITE"\n", result ? COLOR_GREEN : COLOR_RED, result ? "OK" : "KO");

	free(test.retrieved_tokens);

	return (results);
}

void	test_lexical_analyze(int test_number)
{
	t_lexical_test tests[] = {

		// NULL - EMPTY CHECK
		{
			.input = NULL,
			.expected_tokens = NULL,
			.expected_length = 0
		},
		{
			.input = "",
			.expected_tokens = NULL,
			.expected_length = 0
		},

		// NEWLINES
		{
			.input = "\n",
			.expected_tokens = (t_token []) {
				{ .type = TOKEN_NEWLINE }
			},
			.expected_length = 1
		},
		{
			.input = "\n\n\n\n\n",
			.expected_tokens = (t_token []) {
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
			},
			.expected_length = 5
		},
		{
			.input = "   \n   \n    \n   \n   \n   ",
			.expected_tokens = (t_token []) {
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
				{ .type = TOKEN_NEWLINE },
			},
			.expected_length = 5
		},

		// SIMPLE WORDS AND SPACES
		{
			.input = "word1",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "word1"},
			},
			.expected_length = 1
		},
		{
			.input = "word1 word2",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "word1"},
				{.type = TOKEN_WORD, .data.word = "word2"},
			},
			.expected_length = 2
		},
		{
			.input = "word1 word2 word3 word4 word5 word6 word7 word8 word9 word10",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "word1"},
				{.type = TOKEN_WORD, .data.word = "word2"},
				{.type = TOKEN_WORD, .data.word = "word3"},
				{.type = TOKEN_WORD, .data.word = "word4"},
				{.type = TOKEN_WORD, .data.word = "word5"},
				{.type = TOKEN_WORD, .data.word = "word6"},
				{.type = TOKEN_WORD, .data.word = "word7"},
				{.type = TOKEN_WORD, .data.word = "word8"},
				{.type = TOKEN_WORD, .data.word = "word9"},
				{.type = TOKEN_WORD, .data.word = "word10"},
			},
			.expected_length = 10
		},
		{
			.input = " ",
			.expected_tokens = NULL,
		},
		{
			.input = "                           ",
			.expected_tokens = NULL,
		},
		{
			.input = "         word1                  ",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "word1"},
			},
			.expected_length = 1
		},
		{
			.input = "    word1     word2     word3    word4   word5  ",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "word1"},
				{.type = TOKEN_WORD, .data.word = "word2"},
				{.type = TOKEN_WORD, .data.word = "word3"},
				{.type = TOKEN_WORD, .data.word = "word4"},
				{.type = TOKEN_WORD, .data.word = "word5"},
			},
			.expected_length = 5
		},

		// QUOTES
		{
			.input = "'",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'"},
			},
			.expected_length = 1
		},
		{
			.input = "\"",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\""},
			},
			.expected_length = 1
		},
		{
			.input = "''",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "''"},
			},
			.expected_length = 1
		},
		{
			.input = "\"\"",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\"\""},
			},
			.expected_length = 1
		},
		{
			.input = "'                '",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'                '"},
			},
			.expected_length = 1
		},
		{
			.input = "\"                \"",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\"                \""},
			},
			.expected_length = 1
		},
		{
			.input = "\"''''''''''''''''''''\"",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\"''''''''''''''''''''\""},
			},
			.expected_length = 1
		},
		{
			.input = "'\"\"\"\"\"\"\"\"\"\"'",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'\"\"\"\"\"\"\"\"\"\"'"},
			},
			.expected_length = 1
		},

		// QUOTED WORDS
		{
			.input = "'word'",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'word'"},
			},
			.expected_length = 1
		},
		{
			.input = "\"word\"",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\"word\""},
			},
			.expected_length = 1
		},
		{
			.input = "'word1 word2 word3 word4 word5'",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'word1 word2 word3 word4 word5'"},
			},
			.expected_length = 1
		},
		{
			.input = "\"word1 word2 word3 word4 word5\"",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\"word1 word2 word3 word4 word5\""},
			},
			.expected_length = 1
		},
		{
			.input = "   \"word1 word2 word3 word4 word5\" 'word6 word7' '' \"\" '     ' \"     \"   ",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\"word1 word2 word3 word4 word5\""},
				{.type = TOKEN_WORD, .data.word = "'word6 word7'"},
				{.type = TOKEN_WORD, .data.word = "''"},
				{.type = TOKEN_WORD, .data.word = "\"\""},
				{.type = TOKEN_WORD, .data.word = "'     '"},
				{.type = TOKEN_WORD, .data.word = "\"     \""},
			},
			.expected_length = 6
		},
		{
			.input = "'",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'"},
			},
			.expected_length = 1
		},
		{
			.input = "\"",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\""},
			},
			.expected_length = 1
		},
		{
			.input = "'word1 word2 word3 word4 word5",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'word1 word2 word3 word4 word5"},
			},
			.expected_length = 1
		},
		{
			.input = "\"word1 word2 word3 word4 word5",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "\"word1 word2 word3 word4 word5"},
			},
			.expected_length = 1
		},
		{
			.input = "'word1'word2'word3'word4\"word5\"word6\"word7\"word8'word9\"word10\"word11'word12",
			.expected_tokens = (t_token[]) {
				{.type = TOKEN_WORD, .data.word = "'word1'word2'word3'word4\"word5\"word6\"word7\"word8'word9\"word10\"word11'word12"},
			},
			.expected_length = 1
		},

		// PIPES
		{
			.input = "|",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PIPE }
			},
			.expected_length = 1
		},
		{
			.input = "          |              ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PIPE }
			},
			.expected_length = 1
		},
		{
			.input = "          |              |          |          |         |     ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE }
			},
			.expected_length = 5
		},
		{
			.input = "word1|word2|word3|word4|word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},
		{
			.input = "  word1  |  word2  |  word3  |  word4  |  word5  ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},

		// REDIRECTIONS WITHOUT FD
		{
			.input = "<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
			},
			.expected_length = 1
		},
		{
			.input = " < ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
			},
			.expected_length = 1
		},
		{
			.input = ">",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = -1 },
			},
			.expected_length = 1
		},
		{
			.input = " > ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = -1 },
			},
			.expected_length = 1
		},
		{
			.input = "<<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
			},
			.expected_length = 1
		},
		{
			.input = " << ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
			},
			.expected_length = 1
		},
		{
			.input = ">>",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
			},
			.expected_length = 1
		},
		{
			.input = " >> ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
			},
			.expected_length = 1
		},

		// REDIRECTIONS WITH FD
		{
			.input = "1<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = 1 },
			},
			.expected_length = 1
		},
		{
			.input = "0<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = 0 },
			},
			.expected_length = 1
		},
		{
			.input = "9999<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = 9999 },
			},
			.expected_length = 1
		},
		{
			.input = "2147483647<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = 2147483647 },
			},
			.expected_length = 1
		},
		{
			.input = "2147483648<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "2147483648" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
			},
			.expected_length = 2
		},
		{
			.input = "abc<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "abc" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
			},
			.expected_length = 2
		},
		{
			.input = "123abc<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "123abc" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
			},
			.expected_length = 2
		},
		{
			.input = "00000000000000000000000000000000000000000000000000000000000000000123<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = 123 },
			},
			.expected_length = 1
		},
		{
			.input = "\"0\"<",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "\"0\"" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
			},
			.expected_length = 2
		},

		// TOKEN &&
		{
			.input = "&&",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_AND },
			},
			.expected_length = 1
		},
		{
			.input = "   &&       ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_AND },
			},
			.expected_length = 1
		},
		{
			.input = "&&&&",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND }
			},
			.expected_length = 2
		},
		{
			.input = "&&&&&&&&&&&&&&&&&&&&",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND }
			},
			.expected_length = 10
		},
		{
			.input = "word1&&word2&&word3&&word4&&word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},
		{
			.input = "   word1   &&   word2   &&   word3   &&   word4   &&   word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},

		// TOKEN ||
		{
			.input = "||",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_OR },
			},
			.expected_length = 1
		},
		{
			.input = "   ||       ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_OR },
			},
			.expected_length = 1
		},
		{
			.input = "||||",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR }
			},
			.expected_length = 2
		},
		{
			.input = "||||||||||||||||||||",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR }
			},
			.expected_length = 10
		},
		{
			.input = "word1||word2||word3||word4||word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},
		{
			.input = "   word1   ||   word2   ||   word3   ||   word4   ||   word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},

		// TOKEN (
		{
			.input = "(",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
			},
			.expected_length = 1
		},
		{
			.input = "      (     ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
			},
			.expected_length = 1
		},
		{
			.input = "((",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT }
			},
			.expected_length = 2
		},
		{
			.input = "((((((((((",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT }
			},
			.expected_length = 10
		},
		{
			.input = "word1(word2(word3(word4(word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},
		{
			.input = "   word1   (  word2   (   word3   (   word4   (   word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},

		// TOKEN )
		{
			.input = ")",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.expected_length = 1
		},
		{
			.input = "      )     ",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.expected_length = 1
		},
		{
			.input = "))",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT }
			},
			.expected_length = 2
		},
		{
			.input = "))))))))))",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT }
			},
			.expected_length = 10
		},
		{
			.input = "word1)word2)word3)word4)word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},
		{
			.input = "   word1   )  word2   )   word3   )   word4   )   word5",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "word1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word3" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "word5" },
			},
			.expected_length = 9
		},

		// MIXED
		{
			.input = "ls -l | cat 124<Makefile 1235125>>out && sort -r || sleep 5 | (git log -n 5)",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_WORD, .data.word = "-l" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = 124 },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
				{ .type = TOKEN_REDIR_APPEND, .data.number = 1235125 },
				{ .type = TOKEN_WORD, .data.word = "out" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_WORD, .data.word = "-r" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "sleep" },
				{ .type = TOKEN_WORD, .data.word = "5" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "git" },
				{ .type = TOKEN_WORD, .data.word = "log" },
				{ .type = TOKEN_WORD, .data.word = "-n" },
				{ .type = TOKEN_WORD, .data.word = "5" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.expected_length = 21
		},

		{
			.input = "( echo \"user:$USER\" | tr 'a-z' \"A-Z\" ) 3< in.txt && printf '%s\n' 'home:'\"$HOME\" \"$PWD\" | sed 's/x/'\"$XVAR\"'/g' || ( echo \"double:$VAR\" | echo 'single:$VAR' ) 2>err.log 1>out.txt 4>>append.log <<EOF",
			.expected_tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "echo" },
				{ .type = TOKEN_WORD, .data.word = "\"user:$USER\"" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "tr" },
				{ .type = TOKEN_WORD, .data.word = "'a-z'" },
				{ .type = TOKEN_WORD, .data.word = "\"A-Z\"" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_REDIR_INPUT, .data.number = 3 },
				{ .type = TOKEN_WORD, .data.word = "in.txt" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "printf" },
				{ .type = TOKEN_WORD, .data.word = "'%s\n'" },
				{ .type = TOKEN_WORD, .data.word = "'home:'\"$HOME\"" },
				{ .type = TOKEN_WORD, .data.word = "\"$PWD\"" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sed" },
				{ .type = TOKEN_WORD, .data.word = "'s/x/'\"$XVAR\"'/g'" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "echo" },
				{ .type = TOKEN_WORD, .data.word = "\"double:$VAR\"" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "echo" },
				{ .type = TOKEN_WORD, .data.word = "'single:$VAR'" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = 2 },
				{ .type = TOKEN_WORD, .data.word = "err.log" },
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = 1 },
				{ .type = TOKEN_WORD, .data.word = "out.txt" },
				{ .type = TOKEN_REDIR_APPEND, .data.number = 4 },
				{ .type = TOKEN_WORD, .data.word = "append.log" },
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "EOF" },
			},
			.expected_length = 33
		}
	};
	t_test_result	results;

	printf("\nTest %i: lexical_analyze()\n", test_number);

	for (size_t i = 0; i < (sizeof(tests) / sizeof(t_lexical_test)); i++)
	{
		results = conduct_lexical_analysis_test(tests[i]);
	}

	printf("\n==================[RESULTS]==================\n\n");
	printf(COLOR_GREEN "PASS: " COLOR_WHITE "%i/%i\n", results.passed, results.total);
	printf(COLOR_RED "FAIL: " COLOR_WHITE "%i/%i\n", results.failed, results.total);
	printf("\n[TEST %i DONE]\n", test_number);
}

int	main(void)
{
	printf("\nLEXER TESTER\n");

	test_lexical_analyze(1);

	return (EXIT_SUCCESS);
}
