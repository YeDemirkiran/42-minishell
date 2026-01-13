/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 10:03:45 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 19:59:43 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Internal libraries
#include <readline/history.h>
#include <libft/predicate.h>
#include <libft/string.h>

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Checks if is a line only consists of whitespace.
 * @return 0 if line is not empty, 1 otherwise.
 */
static int	is_line_empty(const char *line)
{
	size_t	i;

	if (line == NULL || line[0] == '\0')
		return (1);
	i = 0;
	while (line[i] != '\0')
	{
		if (ft_is_whitespace(line[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Finds the trailing newline character in a string
 * and replaces it with the \0 character.
 * @return The address of the former trailing newline character, now replaced
 * by \0.
 */
static char	*strip_trailing_newline(char *str)
{
	char	*newl_addr;

	newl_addr = ft_strrchr(str, '\n');
	if (newl_addr == NULL)
		return (NULL);
	*newl_addr = '\0';
	return (newl_addr);
}

/**
 * @brief Takes a line, checks if it's not empty and then adds it to
 * the history.
 */
void	add_line_to_history(char *line)
{
	char	*line_no_newl;

	if (is_line_empty(line) == 1)
		return ;
	line_no_newl = strip_trailing_newline(line);
	add_history(line);
	if (line_no_newl != NULL)
		*line_no_newl = '\n';
}

/* ================================== END ================================== */
