/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/18 10:35:43 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libft/predicate.h>
#include <libft/string.h>

#include <enums/e_metachar.h>
#include <enums/e_token_type.h>
#include <structs/s_token.h>

static t_token	create_token(t_token_type type, char *data)
{
	t_token	token;

	token.type = type;
	token.data = data;
	return (token);
}

// Returns true if a given character is a metacharacter.
static int	is_metachar(unsigned char c)
{
	size_t		i;
	const int	metachars[] = {
		METACHAR_SPACE,
		METACHAR_TAB,
		METACHAR_NEWLINE,
		METACHAR_PIPE,
		METACHAR_AMPERSAND,
		METACHAR_SEMICOLON,
		METACHAR_PAREN_LEFT,
		METACHAR_PAREN_RIGHT,
		METACHAR_REDIR_LEFT,
		METACHAR_REDIR_RIGHT
	};

	i = 0;
	while (i < 10)
	{
		if (c == metachars[i])
			return (1);
		i++;
	}
	return (0);
}

// Returns the count of tokens in an input string.
static size_t	get_token_count(char *input)
{
	size_t	i;
	size_t	input_length;
	size_t	token_count;
	char	quote;

	i = 0;
	input_length = ft_strlen(input);
	if (input_length == 0)
		return (0);
	token_count = 0;
	quote = '\0';
	while (i < input_length)
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			if (quote == '\0')
				quote = input[i];
			else if (quote == input[i])
				quote = '\0';
		}
		else if (quote == '\0' && is_metachar(input[i]))
			token_count++;
		else if (quote == '\0' && i > 0 && is_metachar(input[i - 1]))
			token_count++;
		i++;
	}
	token_count++;
	return (token_count);
}

// TODO: Make norm compliant
static t_token	*get_tokens(char *input, size_t token_count)
{
	t_token	*tokens;
	size_t	i;
	size_t	token_index;
	size_t	token_start;
	char	quote;

	tokens = malloc((token_count + 1) * sizeof(t_token));
	if (!tokens)
		return (NULL);
	i = 0;
	token_index = 0;
	token_start = 0;
	quote = '\0';
	while (input[i] != '\0')
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			if (quote == '\0')
				quote = input[i];
			else if (quote == input[i])
				quote = '\0';
		}
		else if (quote == '\0' && is_metachar(input[i]))
		{
			if (i != token_start)
				tokens[token_index++] = create_token(TOKEN_STRING,
						ft_substr(input, token_start, i - token_start));
			if (input[i] == METACHAR_PIPE)
				tokens[token_index].type = TOKEN_PIPE;
			else if (input[i] == METACHAR_REDIR_LEFT || input[i] == METACHAR_REDIR_RIGHT)
				tokens[token_index].type = TOKEN_REDIR;
			else if (input[i] == METACHAR_SPACE || input[i] == METACHAR_TAB)
				tokens[token_index].type = TOKEN_BLANK;
			tokens[token_index++].data = "METACHAR";
			token_start = i + 1;
		}
		i++;
	}
	if (i != token_start)
		tokens[token_index++] = create_token(TOKEN_STRING,
				ft_substr(input, token_start, i - token_start + 1));
	tokens[token_index].type = TOKEN_EOF;
	tokens[token_index].data = NULL;
	return (tokens);
}

static void	debug_print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].type != TOKEN_EOF)
	{
		if (tokens[i].type == TOKEN_BLANK)
			printf("[BLANK]");
		else if (tokens[i].type == TOKEN_REDIR)
			printf("[REDIR]");
		else if (tokens[i].type == TOKEN_PIPE)
			printf("[PIPE]");
		else
			printf("%s", tokens[i].data);
		if (tokens[i + 1].type != TOKEN_EOF)
			printf(", ");
		fflush(stdout);
		i++;
	}
	printf(")\n");
}

// Performs lexical analysis of an input and returns an array of tokens.
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
	return (tokens);
}
