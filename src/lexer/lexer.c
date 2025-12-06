/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/06 14:29:59 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <libft/predicate.h>
#include <libft/string.h>
#include <libft/linked_list.h>

#include "enums/e_metachar.h"
#include "structs/s_token.h"

#include "lexer_utils.h"
#include "token_utils.h"

/**
 * @brief Turns a string to a valid file descriptor number.
 * @returns The resulted file descriptor or,
 * @returns -1 if the resulted number is not a valid file descriptor.
 */
static int	get_valid_fd_from_string(char *string)
{
	size_t	i;
	size_t	fd;

	if (string == NULL || string[0] == '\0')
		return (-1);
	fd = 0;
	i = 0;
	while (string[i] == '0')
		i++;
	if (ft_strlen(string + i) > 10)
		return (-1);
	while (string[i] != '\0')
	{
		if (ft_isdigit(string[i]) != 1)
			return (-1);
		fd *= 10;
		fd += string[i] - '0';
		i++;
	}
	if (fd > INT_MAX)
		return (-1);
	return (fd);
}

/**
 * @brief Converts a string stream to word token, starting from index.
 * @returns 0 on failure, 1 on success,
 * 2 if any parameter is NULL or START == END.
 */
static int	save_word_token(t_list **tokens, char *string)
{
	t_token			*token;
	t_token_data	data;

	if (tokens == NULL || string == NULL)
		return (2);
	data.word = string;
	token = create_token(TOKEN_WORD, data);
	if (token == NULL)
	{
		free(data.word);
		return (0);
	}
	if (save_token_to_list(tokens, token) != 1)
	{
		free_token(token);
		return (0);
	}
	return (1);
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

/**
 * @brief Converts a string stream to token, starting from index.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
static int	save_metachar_token(t_list **tokens,
		char *string, size_t word_start, size_t *index)
{
	t_token			*token;
	size_t			token_length;
	t_token_type	type;
	t_token_data	data;
	char			*word;

	if (tokens == NULL || string == NULL || index == NULL)
		return (2);
	word = NULL;
	if (*index > word_start)
	{
		word = ft_substr(string, word_start, *index - word_start);
		if (word == NULL)
			return (0);
	}
	type = get_token_type(string + *index, &token_length);
	*index += token_length;
	if (is_token_redir(type))
	{
		if (word != NULL)
		{
			data.number = get_valid_fd_from_string(word);
			if (data.number == -1)
			{
				if (save_word_token(tokens, word) != 1)
					return (0);
			}
			else
			{
				free(word);
			}
		}
		else
		{
			data.number = -1;
		}
	}
	else
	{
		if (word != NULL)
		{
			if (save_word_token(tokens, word) != 1)
				return (0);
		}
		data.word = NULL;
	}
	if (type == TOKEN_NONE)
		return (1);
	token = create_token(type, data);
	if (token == NULL)
		return (0);
	if (save_token_to_list(tokens, token) != 1)
	{
		free_token(token);
		return (0);
	}
	return (1);
}

/**
 * @brief Returns tokens from a given string.
 * @returns NULL on failure.
 */
static t_list	*get_tokens(char *string)
{
	t_list	*tokens;
	size_t	i;
	size_t	token_start;
	char	quote;

	if (string == NULL)
		return (NULL);
	tokens = NULL;
	i = 0;
	token_start = 0;
	quote = '\0';
	while (string[i] != '\0')
	{
		if (string[i] == '#' && quote == '\0')
			break ;
		if (string[i] == '"' || string[i] == '\'')
			set_quote(&quote, string[i]);
		if (quote == '\0' && !(string[i] == '&' && string[i + 1] != '&')
			&& is_metachar(string[i]))
		{
			//save_metachar_token(&tokens, string, token_start, &i);
			if (save_metachar_token(&tokens, string, token_start, &i) != 1)
			{
				free_tokens(&tokens);
				return (NULL);
			}
			token_start = i;
		}
		else
			i++;
	}
	return (tokens);
}

// TODO: Remove this function
static void	debug_print_tokens(t_list *tokens)
{
	t_token	*token;

	while (tokens != NULL)
	{
		token = (t_token *)(tokens->content);
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
		if (tokens->next != NULL)
			printf(", ");
		tokens = tokens->next;
	}
}

// Performs lexical analysis of a string and returns an array of tokens.
// TODO: Remove debug print statements
t_list	*lexical_analyze(char *string)
{
	t_list	*tokens;

	printf("(DEBUG: Starting shell step 1: Lexical Analysis & Tokenization)\n");
	printf("(DEBUG: Step 1: Given string: %s)\n", string);
	if (string == NULL)
		return (NULL);
	printf("(DEBUG: Step 1.1: Retrieved tokens: ");
	tokens = get_tokens(string);
	if (tokens == NULL)
	{
		printf("NULL)\n");
		return (NULL);
	}
	debug_print_tokens(tokens);
	printf(")\n");
	printf("(DEBUG: Step 1 'Lexical Analysis & Tokenization' completed.)\n");
	return (tokens);
}
