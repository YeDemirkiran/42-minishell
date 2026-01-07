/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:44:55 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/07 22:00:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// External libraries
#include <stdlib.h>

// Internal libraries
#include <libft/string.h>

// Project headers
#include "enums/e_metachar.h"
#include "structs/s_token.h"
#include "types/t_bool.h"

/* =============================== FUNCTIONS =============================== */

/**
* @brief Checks whether if a given character is a metacharacter.
* @returns 0 if the character is not a metacharacter, 1 if it is.
*/
t_bool	is_metachar(unsigned char c)
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

// /**
//  * @brief Sets the quote char.
// */
// void	set_quote(char *quote_char, char quote_target)
// {
// 	if (quote_char == NULL)
// 		return ;
// 	if (*quote_char == '\0')
// 		*quote_char = quote_target;
// 	else if (*quote_char == quote_target)
// 		*quote_char = '\0';
// }

/**
 * @brief Returns the one-character token type of a string, or TOKEN_NONE.
*/
static t_token_type	get_token_type_one_char(char c)
{
	t_token_type	type;

	if (c == '<')
		type = TOKEN_REDIR_INPUT;
	else if (c == '>')
		type = TOKEN_REDIR_OUTPUT;
	else if (c == '\n')
		type = TOKEN_NEWLINE;
	else if (c == '|')
		type = TOKEN_PIPE;
	else if (c == '(')
		type = TOKEN_PAREN_LEFT;
	else if (c == ')')
		type = TOKEN_PAREN_RIGHT;
	else
		type = TOKEN_NONE;
	return (type);
}

/**
 * @brief Returns the two-characters token type of a string, or TOKEN_NONE.
*/
static t_token_type	get_token_type_two_char(char *string)
{
	t_token_type	type;

	if (ft_strncmp(string, "<<", 2) == 0)
		type = TOKEN_REDIR_HEREDOC;
	else if (ft_strncmp(string, ">>", 2) == 0)
		type = TOKEN_REDIR_APPEND;
	else if (ft_strncmp(string, "&&", 2) == 0)
		type = TOKEN_IF_AND;
	else if (ft_strncmp(string, "||", 2) == 0)
		type = TOKEN_IF_OR;
	else
		type = TOKEN_NONE;
	return (type);
}

/**
 * @brief Returns the token type of a string.
*/
t_token_type	get_token_type(char *string, size_t *token_length)
{
	t_token_type	type;

	type = get_token_type_two_char(string);
	if (type != TOKEN_NONE)
	{
		if (token_length != NULL)
			*token_length = 2;
		return (type);
	}
	type = get_token_type_one_char(*string);
	if (token_length != NULL)
		*token_length = 1;
	return (type);
}

/* ================================== END ================================== */
