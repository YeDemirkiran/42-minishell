/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word_count.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:53:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:37:03 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

// Returns the word count in a string separated by char c.
size_t	ft_word_count(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (i && ((s[i] == c && s[i - 1] != c)
				|| (s[i] != c && !s[i + 1])))
			count++;
		i++;
	}
	return (count);
}
