/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/07 18:07:41 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// External libraries
#include <stdlib.h>

// Internal libraries
#include <libft/string.h>
#include <libft/linked_list.h>

// Project headers
#include "structs/s_token.h"

// Local headers
#include "lexer_utils.h"
#include "lexer_token.h"
#include "token_utils.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes the word pointer and sets it to the substring of string.
 * @returns 0 on failure, 1 on success.
 */
static int	set_word(char **word, char *string,
	size_t start, size_t end)
{
	*word = NULL;
	if (end > start)
	{
		*word = ft_substr(string, start, end - start);
		if (*word == NULL)
			return (0);
	}
	return (1);
}

/**
 * @brief Converts a string stream to token, starting from index.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
static int	save_metachar_token(t_list **tokens,
		char *string, size_t word_start, size_t *index)
{
	t_token	token;
	t_token	*token_addr;
	size_t	token_length;
	char	*word;

	if (set_word(&word, string, word_start, *index) != 1)
		return (0);
	token.type = get_token_type(string + *index, &token_length);
	*index += token_length;
	if (set_token_data(tokens, &token, word) != 1)
		return (0);
	if (token.type == TOKEN_NONE)
		return (1);
	token_addr = create_token(token.type, token.data);
	if (token_addr == NULL)
		return (0);
	if (save_token_to_list(tokens, token_addr) != 1)
	{
		free_token(token_addr);
		return (0);
	}
	return (1);
}

/**
 * Initializes the lexer parameters.
 */
static void	lexer_init(t_list **tokens, size_t *index,
	size_t *token_start, char *quote_char)
{
	*tokens = NULL;
	*index = 0;
	*token_start = 0;
	*quote_char = '\0';
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

	lexer_init(&tokens, &i, &token_start, &quote);
	while (string[i] != '\0')
	{
		if (string[i] == '#' && quote == '\0' && token_start == i)
			break ;
		if (string[i] == '"' || string[i] == '\'')
			set_quote(&quote, string[i]);
		if (quote == '\0' && is_metachar(string[i]))
		{
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
#include "stdio.h"
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

/**
 * @brief Performs lexical analysis on a given string and returns tokens.
 * @returns A linked list of tokens on success, NULL on failure.
 */
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

/* ================================== END ================================== */
