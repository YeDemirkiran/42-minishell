/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:44:55 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/03 13:25:40 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include <libft/string.h>
#include <enums/e_metachar.h>
#include <structs/s_token.h>
#include <types/t_bool.h>

// Returns true if a given character is a metacharacter.
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

// Creates a token of type and with data.
t_token	*create_token(t_token_type type, t_token_data data)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->data = data;
	return (token);
}
