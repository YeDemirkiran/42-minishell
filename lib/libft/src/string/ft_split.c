/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:49:56 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:26:09 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include "libft/string.h"

static void	*clear_alloc(char **s, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		free(s[i++]);
	free(s);
	return (NULL);
}

// Splits a string by delimiter char c and returns an array of strings.
char	**ft_split(char const *s, char c)
{
	char	**str_arr;
	size_t	len;
	size_t	j;

	str_arr = malloc((ft_word_count(s, c) + 1) * sizeof(char *));
	if (!str_arr)
		return (NULL);
	j = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (!*s)
			break ;
		len = 0;
		while (s[len] && s[len] != c)
			len++;
		str_arr[j] = malloc(len + 1);
		if (!str_arr[j])
			return (clear_alloc(str_arr, j));
		ft_strlcpy(str_arr[j++], s, len + 1);
		s += len;
	}
	str_arr[j] = NULL;
	return (str_arr);
}
