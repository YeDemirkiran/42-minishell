/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:50:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 21:12:05 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include "libft/string.h"

// Returns a duplicate of a string within 'start' and 'start + len' range.
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	str_rmng;
	size_t	size;

	if (ft_strlen(s) < start)
		str_rmng = 0;
	else
		str_rmng = ft_strlen(s + start);
	if (str_rmng < len)
		size = str_rmng + 1;
	else
		size = len + 1;
	substr = malloc(size * sizeof(*substr));
	if (!substr)
		return (NULL);
	if (size == 1)
		*substr = 0;
	else
		ft_strlcpy(substr, s + start, size);
	return (substr);
}
