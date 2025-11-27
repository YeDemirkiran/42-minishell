/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/27 18:05:16 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libft/predicate.h>
#include <libft/string.h>

#include <enums/e_metachar.h>
#include <enums/e_token_type.h>
#include <structs/s_token.h>

#include "lexer_utils.h"

// Converts char to token
static t_token	get_metachar_token(char c)
{
	t_token	token;

	if (c == METACHAR_SPACE || c == METACHAR_TAB)
		token.type = TOKEN_BLANK;
	else if (c == METACHAR_NEWLINE)
		token.type = TOKEN_NEWLINE;
	else if (c == METACHAR_PIPE)
		token.type = TOKEN_PIPE;
	else if (c == METACHAR_REDIR_LEFT || c == METACHAR_REDIR_RIGHT)
		token.type = TOKEN_REDIR;
	else if (c == METACHAR_PAREN_LEFT || c == METACHAR_PAREN_RIGHT)
		token.type = TOKEN_PARENTHESIS;
	else if (c == METACHAR_AMPERSAND)
		token.type = TOKEN_AMPERSAND;
	token.data = NULL;
	return (token);
}

// Adds a new string token to a token list
static void	save_string_token(t_token *tokens, size_t *index,
	char *start, char *end)
{
	if (tokens == NULL || index == NULL || start == end)
		return ;
	tokens[*index].type = TOKEN_STRING;
	tokens[*index].data = ft_substr(start, 0, (size_t)(end - start));
	(*index)++;
}

// Helper function to initialize token parameters.
static void	init_token_params(size_t *index, size_t *t_index,
	size_t *t_start, char *quote)
{
	*index = 0;
	*t_index = 0;
	*t_start = 0;
	*quote = '\0';
}

// Returns tokens from a given string.
// TODO: Make norm compliant
static t_token	*get_tokens(char *input, size_t token_count)
{
	t_token	*tokens;
	size_t	i;
	size_t	t_index;
	size_t	t_start;
	char	q;

	tokens = malloc((token_count + 1) * sizeof(t_token));
	if (!tokens)
		return (NULL);
	init_token_params(&i, &t_index, &t_start, &q);
	while (input[i] != '\0')
	{
		if (input[i] == '"' || input[i] == '\'')
			q = ((q == '\0') * input[i]) + ((q != input[i]) * q);
		if (q == '\0' && is_metachar(input[i]))
		{
			save_string_token(tokens, &t_index, input + t_start, input + i);
			tokens[t_index++] = get_metachar_token(input[i]);
			t_start = i + 1;
		}
		i++;
	}
	save_string_token(tokens, &t_index, input + t_start, input + i);
	tokens[t_index] = create_token(TOKEN_EOF, NULL);
	return (tokens);
}

// TODO: Remove this function
static void	debug_print_tokens(t_token *tokens)
{
	int	i;

	if (tokens == NULL)
	{
		printf("(NULL)\n");
		return ;
	}
	i = 0;
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
}

// Performs lexical analysis of an input and returns an array of tokens.
// TODO: Remove debug print statements
t_token	*lexical_analyze(char *input)
{
	size_t	token_count;
	t_token	*tokens;

	printf("(DEBUG: Starting shell step 1: Lexical Analysis & Tokenization)\n");
	printf("(DEBUG: Step 1: Given input: %s)\n", input);
	if (input == NULL)
		return (NULL);
	token_count = get_token_count(input);
	printf("(DEBUG: Step 1.1: Lexer token count: %lu)\n", token_count);
	printf("(DEBUG: Step 1.2: Retrieved tokens: ");
	tokens = get_tokens(input, token_count);
	if (tokens == NULL)
		return (NULL);
	debug_print_tokens(tokens);
	printf(")\n");
	printf("(DEBUG: Step 1 'Lexical Analysis & Tokenization' completed.)\n");
	return (tokens);
}
