/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:22:02 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:34:00 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// External libraries
#include <stdlib.h>

// Internal libraries
#include <libft/linked_list.h>

// Project headers
#include "structs/s_token.h"
#include "types/t_bool.h"

// Local headers
#include "token.h"
#include "lexer_utils.h"
#include "lexer_misc.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Converts a string stream to word token, starting from index.
 * @returns 0 on failure, 1 on success,
 * 2 if any parameter is NULL or START == END.
 * @note The string parameter is automatically freed on failure.
 */
int	save_word_token(t_list **tokens, char *string)
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

/**
 * @brief Checks whether the type parameter is a redirection token.
 */
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
 * @brief Sets redirection token on certain conditions.
 * @param tokens Is used if the word is going to be used as a word token.
 * @param token_addr Pointer to the local token struct that will be filled.
 * @param word Is checked if it contains a valid FD number.
 * @returns 0 on failure, 1 on success.
 * @note If the word parameter doesn't contain a valid file descriptor number,
 * it is saved as a normal word token instead.
 */
static t_bool	set_redir_token_data(t_list **tokens,
		t_token *token_addr, char *word)
{
	if (word != NULL)
	{
		token_addr->data.number = get_valid_fd_from_string(word);
		if (token_addr->data.number == -1)
		{
			if (save_word_token(tokens, word) != 1)
				return (0);
		}
		else
			free(word);
	}
	else
		token_addr->data.number = -1;
	return (1);
}

/**
 * @brief Sets the token data according to the token type.
 * @param tokens Used for word token saving if no type is suitable.
 * @param token_addr Pointer to the local token struct that will be filled.
 * @param word The string that will be used as the source of the data.
 * @returns 0 on failure, 1 on success.
 * @note - If the word doesn't fit any token type, it is saved as a word.
 * @note - Currently only checks for redirections.
 */
int	set_token_data(t_list **tokens, t_token *token_addr, char *word)
{
	if (is_token_redir(token_addr->type))
	{
		if (set_redir_token_data(tokens, token_addr, word) != 1)
			return (0);
	}
	else
	{
		if (word != NULL)
			if (save_word_token(tokens, word) != 1)
				return (0);
		token_addr->data.word = NULL;
	}
	return (1);
}

/**
 * @brief Converts a string stream to token, starting from index.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
int	save_metachar_token(t_list **tokens,
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

/* ================================== END ================================== */
