/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/19 11:16:21 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Internal libraries
#include <libft/string.h>
#include <libft/linked_list.h>

// Project headers
#include "structs/s_token.h"

// Local headers
#include "lexer_utils.h"
#include "lexer_misc.h"
#include "token.h"
#include "token_utils.h"

/* =============================== FUNCTIONS =============================== */

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
 * @brief Is called when the current character is a metachar.
 * @return 0 on failure, 1 on success.
 * @note Sets token_start to index on success.
 * @note Automatically frees the whole tokens list on failure.
 */
static int	on_metachar(t_list **tokens, char *string,
	size_t *token_start_ptr, size_t *index_ptr)
{
	if (save_metachar_token(tokens, string, *token_start_ptr, index_ptr) != 1)
	{
		free_tokens(tokens);
		return (0);
	}
	*token_start_ptr = *index_ptr;
	return (1);
}

/**
 * @brief Generates and saves a word in a token list.
 * @returns 0 on failure, 1 on success.
 */
static int	save_word(t_list **tokens, char *string, size_t start, size_t end)
{
	char	*word;

	if (set_word(&word, string, start, end) != 1)
		return (0);
	if (word == NULL)
		return (1);
	if (save_word_token(tokens, word) != 1)
		return (0);
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

	lexer_init(&tokens, &i, &token_start, &quote);
	while (string[i] != '\0')
	{
		if (string[i] == '#' && quote == '\0' && token_start == i)
			break ;
		if (string[i] == '"' || string[i] == '\'')
			set_quote(&quote, string[i]);
		if (quote == '\0' && is_metachar(string[i]))
		{
			if (on_metachar(&tokens, string, &token_start, &i) != 1)
				return (NULL);
		}
		else
			i++;
	}
	if (save_word(&tokens, string, token_start, i) != 1)
		free_tokens(&tokens);
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

	//printf("(DEBUG: Starting shell step 1: Lexical Analysis & Tokenization)\n");
	//printf("(DEBUG: Step 1: Given string: %s)\n", string);
	if (string == NULL)
		return (NULL);
	//printf("(DEBUG: Step 1.1: Retrieved tokens: ");
	tokens = get_tokens(string);
	if (tokens == NULL)
	{
		printf("NULL)\n");
		return (NULL);
	}
	//debug_print_tokens(tokens);
	//printf(")\n");
	//printf("(DEBUG: Step 1 'Lexical Analysis & Tokenization' completed.)\n");
	return (tokens);
}

/* ================================== END ================================== */
