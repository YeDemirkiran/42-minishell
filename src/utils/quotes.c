/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 21:46:49 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:35:47 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stddef.h>

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes a character and a pointer to a quote character,
 * and sets it according to the value of the C character.
 *
 * - If C is a single quote ('), and the current quote is not a
 * double quote ("), then the current quote is either set to a single quote or
 * the \0 character.
 *
 * - Vice versa for double quotes.
 */
void	set_quote(char c, char *quote_addr)
{
	if (quote_addr == NULL)
		return ;
	if (c == '\'' && *quote_addr != '"')
	{
		if (*quote_addr == '\'')
			*quote_addr = 0;
		else
			*quote_addr = '\'';
	}
	else if (c == '"' && *quote_addr != '\'')
	{
		if (*quote_addr == '"')
			*quote_addr = 0;
		else
			*quote_addr = '"';
	}
}

/* ================================== END ================================== */
