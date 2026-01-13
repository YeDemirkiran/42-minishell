/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 15:30:24 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 20:07:28 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stddef.h>
#include <unistd.h>

// Internal libraries
#include <libft/write.h>

// Project headers
#include "modules/utils.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Checks if an input has unclosed quotes, and returns the type of
 * quote.
 * @return 0 (null) character if input is complete, and the unmatched quote
 * character if it's not.
 */
char	is_input_incomplete(const char *input)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i] != '\0')
	{
		set_quote(input[i], &quote);
		i++;
	}
	return (quote);
}

/**
 * @brief Prints an EOF error message to stderr depending on the given
 * quote character.
 */
void	print_uncompleted_eof_error(char quote)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching ",
		STDERR_FILENO);
	if (quote == '"')
		ft_putstr_fd("'\"'", STDERR_FILENO);
	else
		ft_putstr_fd("(')", STDERR_FILENO);
	ft_putstr_fd("\nminishell: syntax error: unexpected end of file\n",
		STDERR_FILENO);
}

/* ================================== END ================================== */
