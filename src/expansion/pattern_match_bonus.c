/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_match_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:42:37 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:22:08 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Internal libraires
#include <libft/string.h>

// Project headers
#include "modules/utils.h"

// Local headers
#include "remove_quotes.h"
#include "glob_bonus.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes a buffer and a word, and tries to find it in
 * the given name.
 *
 * If first_occurrence is positive, the first occurrence of the buffer is
 * returned. Otherwise, the last occurrence is returned.
 */
static char	*find_buffer_in_name(const char *name,
	const char *buff, int first_occurrence)
{
	const char	*previous;
	const char	*current;

	if (name == NULL || buff == NULL)
		return (NULL);
	previous = NULL;
	current = name;
	while (*current != '\0')
	{
		current = ft_strnstr(current, buff, ft_strlen(current));
		if (current == NULL)
			break ;
		else if (first_occurrence > 0)
			return ((char *)(current));
		previous = current;
		current = current + 1;
	}
	return ((char *)previous);
}

static int	pattern_match_initial_check(const char *pattern, const char *name,
	t_glob_flags flags)
{
	int	expand_hidden;
	int	add_local_prefix;

	if (pattern == NULL || name == NULL)
		return (0);
	expand_hidden = (flags & GLOB_FLAG_EXPAND_HIDDEN);
	add_local_prefix = (flags & GLOB_FLAG_ADD_LOCAL_PREFIX);
	if (!expand_hidden && !add_local_prefix && name[0] == '.')
		return (0);
	return (1);
}

static void	pattern_match_prepare_buffer(char *buffer, const char *pattern,
	size_t *i, size_t buffer_start)
{
	char	quote;

	quote = 0;
	while ((quote != 0 || pattern[*i] != '*') && pattern[*i] != '\0')
		set_quote(pattern[(*i)++], &quote);
	ft_strlcpy(buffer, pattern + buffer_start, *i - buffer_start + 1);
	remove_quotes(buffer);
}

/**
 * @brief Takes a pattern and a name, and checks if they do match.
 *
 * @return 1 if pattern matches, 0 otherwise.
 */
int	pattern_match(const char *pattern, const char *name, t_glob_flags flags)
{
	size_t	i;
	size_t	buffer_start;
	char	*current;
	char	buffer[GLOB_BUFFER_SIZE];

	if (pattern_match_initial_check(pattern, name, flags) != 1)
		return (0);
	i = 0;
	current = (char *)name;
	while (1)
	{
		while (pattern[i] == '*')
			i++;
		if (pattern[i] == '\0')
			break ;
		buffer_start = i;
		pattern_match_prepare_buffer(buffer, pattern, &i, buffer_start);
		current = find_buffer_in_name(current, buffer, buffer_start == 0);
		if (current == NULL || (buffer_start == 0 && current != name))
			return (0);
		else if (pattern[i] == '\0' && current[ft_strlen(buffer)] != '\0')
			return (0);
		current += i - buffer_start;
	}
	return (1);
}

/* ================================== END ================================== */
