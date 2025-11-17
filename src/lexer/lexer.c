/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/17 17:44:32 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <libft/predicate.h>
#include <libft/string.h>

#include <enums/e_metachar.h>
#include <enums/e_token_type.h>
#include <structs/s_token.h>

static unsigned char	ft_isquote(unsigned char c)
{
	return (c == '\'' || c == '"');
}

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
	size_t	token_count;
	char	quote_type;
	size_t	i;

	i = 0;
	token_count = 0;
	while (input[i])
	{
		while (input[i] && ft_is_whitespace(input[i]))
			i++;
		while (input[i])
		{
			while (ft_isquote(input[i]))
			{
				quote_type = input[i];
				i++;
				while (input[i] && input[i] != quote_type)
					i++;
				i++;
			}
			if (ft_is_whitespace(input[i]))
				break ;
			i++;
		}
		token_count++;
	}
	return (token_count);
}

// char	*ft_substr(char *str, size_t start, size_t length)
// {
// 	char	*new_str;
// 	size_t	i;

// 	// todo: Use garbage collector here
// 	new_str = malloc((length + 1) * sizeof(char));
// 	if (new_str == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (i < length)
// 	{
// 		new_str[i] = str[start + i];
// 		i++;
// 	}
// 	new_str[i] = '\0';
// 	//printf("(new substr: %s)\n", new_str);
// 	return (new_str);
// }

// static t_token	*get_tokens(char *input, size_t token_count)
// {
// 	size_t	i;
// 	size_t	token_index;
// 	t_token	*tokens;
// 	size_t	current_token_length;
// 	char	*current_token_start;
// 	char	quote_type;

// 	tokens = malloc((token_count + 1) * sizeof(t_token));
// 	if (tokens == NULL)
// 		return (NULL);
// 	tokens[token_count].type = TOKEN_EOF;
// 	i = 0;
// 	token_index = 0;
// 	while (input[i])
// 	{
// 		while (input[i] && ft_is_whitespace(input[i]))
// 			i++;
// 		if (input[i] == '\0')
// 			break ;
// 		current_token_length = 0;
// 		current_token_start = input + i;
// 		while (input[i])
// 		{
// 			while (ft_isquote(input[i]))
// 			{
// 				quote_type = input[i];
// 				i++;
// 				while (input[i] && input[i] != quote_type)
// 					i++;
// 				i++;
// 			}
// 			if (ft_is_whitespace(input[i]))
// 				break ;
// 			i++;
// 		}
// 		current_token_length = (size_t)((input + i) - current_token_start);
// 		// todo: Null check
// 		tokens[token_index].data = ft_substr(current_token_start, 0, current_token_length);
// 		tokens[token_index].type = get_token_type(tokens[token_index].data);
// 		token_index++;
// 	}
// 	return (tokens);
// }

// static void	create_token(t_token *token)

// TODO: Make norm compliant
static t_token	*get_tokens(char *input, size_t token_count)
{
	// get_tokens
	size_t	i;
	size_t	input_length;
	t_token	*tokens;

	// token specific
	size_t	token_index;
	size_t	token_start;
	t_token	current_token;
	
	// quotes
	char	quote_type;

	tokens = malloc((token_count + 1) * sizeof(t_token));
	if (!tokens)
		return (NULL);
	i = 0;
	input_length = ft_strlen(input);
	token_index = 0;
	token_start = 0;
	quote_type = '\0';
	while (i < input_length)
	{
		if (ft_isquote(input[i]))
		{			
			if (quote_type == '\0')
			{
				if (i > 0)
				{
					current_token.type = TOKEN_STRING;
					current_token.data = ft_substr(input, (unsigned int)token_start, i - token_start + 1);
					tokens[token_index++] = current_token;
				}
				token_start = i;
				quote_type = input[i];
			}
			else
			{
				if (input[i] == quote_type)
				{
					current_token.type = TOKEN_QUOTE;
					current_token.data = ft_substr(input, (unsigned int)token_start, i - token_start + 1);
					tokens[token_index++] = current_token;
					token_start = i + 1;
					quote_type = '\0';
				}
			}
		}
		else if (is_metachar(input[i]))
		{	
			// save the word so far
			current_token.type = TOKEN_STRING;
			current_token.data = ft_substr(input, (unsigned int)token_start, i - token_start + 1);
			tokens[token_index++] = current_token;
			if (input[i] == METACHAR_PIPE)
				current_token.type = TOKEN_PIPE;
			else if (input[i] == METACHAR_REDIR_LEFT || input[i] == METACHAR_REDIR_RIGHT)
				current_token.type = TOKEN_REDIR;
			else if (input[i] == METACHAR_SPACE || input[i] == METACHAR_TAB)
				current_token.type = TOKEN_BLANK;
			current_token.data = NULL;
			tokens[token_index++] = current_token;
			token_start = i + 1;
		}
		i++;
	}
	if (i != token_start)
	{
		current_token.type = TOKEN_STRING;
		current_token.data = ft_substr(input, (unsigned int)token_start, i - token_start + 1);
		tokens[token_index++] = current_token;
	}
	current_token.type = TOKEN_EOF;
	current_token.data = NULL;
	tokens[token_index++] = current_token;
	return (tokens);
}

void	debug_print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].type != TOKEN_EOF)
	{
		printf("[%i]: %s", tokens[i].type, tokens[i].data);
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
