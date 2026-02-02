/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:45:50 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/02 10:58:12 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <libft/string.h>

#include "modules/utils.h"

#include "remove_quotes.h"
#include "glob_bonus.h"

static int	is_pattern_dir(const char *pattern)
{
	size_t	i;

	i = ft_strlen(pattern) - 1;
	while (i > 0 && pattern[i] == '*')
		i--;
	if (i > 0 && pattern[i] == '/')
		return (1);
	return (0);
}

/**
 * @brief Initializes the given glob struct's flags and other parameters.
 */
void	set_glob_struct(const char *pattern, t_glob *glob)
{
	size_t	i;
	char	c;
	char	quote;

	i = 0;
	quote = 0;
	glob->result = GLOB_NO_NEED;
	glob->flags = GLOB_FLAG_NONE;
	glob->word_list = NULL;
	while (pattern[i] != '\0')
	{
		c = pattern[i];
		if (i == 0 && c == '.' && pattern[1] == '/')
			glob->flags |= GLOB_FLAG_ADD_LOCAL_PREFIX;
		else if (i == 0 && c == '.')
			glob->flags |= GLOB_FLAG_EXPAND_HIDDEN;
		set_quote(c, &quote);
		if (c == '*' && quote == 0)
			glob->result = GLOB_SUCCESS;
		i++;
	}
	if (is_pattern_dir(pattern) == 1)
		glob->flags |= GLOB_FLAG_EXPAND_DIRS;
	if (i >= GLOB_BUFFER_SIZE)
		glob->result = GLOB_NO_NEED;
}
