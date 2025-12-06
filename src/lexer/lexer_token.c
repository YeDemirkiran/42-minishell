/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:13:53 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/06 23:38:52 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <limits.h>

#include <libft/linked_list.h>
#include <libft/predicate.h>
#include <libft/string.h>

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
