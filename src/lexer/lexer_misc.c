/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_misc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:13:53 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 16:25:46 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// External libraries
#include <stddef.h>
#include <limits.h>

// Internal libraries
#include "libft/linked_list.h"
#include "libft/predicate.h"
#include "libft/string.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Turns a string to a valid file descriptor number.
 * @returns The resulted file descriptor or,
 * @returns -1 if the resulted number is not a valid file descriptor.
 */
int	get_valid_fd_from_string(char *string)
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
 * @brief Takes the word pointer and sets it to the substring of string.
 * @returns 0 on failure, 1 on success.
 */
int	set_word(char **word, char *string, size_t start, size_t end)
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

/* ================================== END ================================== */
