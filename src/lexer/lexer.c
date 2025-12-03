/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/03 19:32:33 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libft/predicate.h>
#include <libft/string.h>
#include <libft/linked_list.h>

#include "enums/e_metachar.h"
#include "structs/s_token.h"

#include "lexer_utils.h"
#include "token_utils.h"

static t_token_data	get_token_data(char *string, t_token_type type)
{
	t_token_data	data;

	(void)string;
	(void)type;
	data.word = NULL;
	return (data);
}

/**
 * @brief Converts a string stream to token, starting from index.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
static t_bool	save_metachar_token(t_list **tokens,
		char *string, size_t *index)
{
	t_token			*token;
	t_token_type	type;
	t_token_data	data;

	if (tokens == NULL || string == NULL || index == NULL)
		return (2);
	type = get_token_type(string, index);
	if (type == TOKEN_NONE)
		return (1);
	data = get_token_data(string, type);
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
 * @brief Converts a string stream to word token, starting from index.
 * @returns 0 on failure, 1 on success,
 * 2 if any parameter is NULL or START == END.
 */
static t_bool	save_word_token(t_list **tokens,
	char *start, char *end)
{
	t_token			*token;
	t_token_data	data;

	if (tokens == NULL || start == end)
		return (2);
	data.word = ft_substr(start, 0, (size_t)(end - start));
	if (data.word == NULL)
		return (0);
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

// Returns tokens from a given string.
//
// Returns NULL on failure.
static t_list	*get_tokens(char *string)
{
	t_list	*tokens;
	size_t	i;
	size_t	t_start;
	char	quote;

	tokens = NULL;
	i = 0;
	t_start = 0;
	quote = '\0';
	while (string[i] != '\0')
	{
		if (string[i] == '"' || string[i] == '\'')
			set_quote(&quote, string[i]);
		if (quote == '\0' && is_metachar(string[i]))
		{
			save_word_token(&tokens, string + t_start, string + i);
			save_metachar_token(&tokens, string, &i);
			t_start = i;
		}
		else
			i++;
	}
	save_word_token(&tokens, string + t_start, string + i);
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
			printf("[REDIR_INPUT]");
		else if (token->type == TOKEN_REDIR_OUTPUT)
			printf("[REDIR_OUTPUT]");
		else if (token->type == TOKEN_REDIR_HEREDOC)
			printf("[REDIR_HEREDOC]");
		else if (token->type == TOKEN_REDIR_APPEND)
			printf("[REDIR_APPEND]");
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
