/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:44:55 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/19 11:27:36 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

#include <libft/string.h>
#include <enums/e_metachar.h>
#include <enums/e_token_type.h>
#include <structs/s_token.h>

// Returns true if a given character is a metacharacter.
int	is_metachar(unsigned char c)
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
t_token	create_token(t_token_type type, char *data)
{
	t_token	token;

	token.type = type;
	token.data = data;
	return (token);
}

// Returns the count of tokens in an input string.
// TODO: Make Norm compliant
size_t	get_token_count(char *input)
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
			quote = ((quote == '\0') * input[i])
				+ ((quote != input[i]) * quote);
		if (quote == '\0' && (is_metachar(input[i])
				|| (i + 1 < input_length && is_metachar(input[i + 1]))))
			token_count++;
		i++;
	}
	if (i > 0 && !is_metachar(input[i - 1]))
		token_count++;
	return (token_count);
}
