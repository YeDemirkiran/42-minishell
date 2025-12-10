/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:50:13 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 21:11:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include "libft/string.h"

// Iteratively applies function 'f' to each character of a string and returns
// the copy.
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;
	size_t			str_len;

	str_len = ft_strlen(s) + 1;
	str = malloc(str_len * sizeof(*str));
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = (*f)(i, s[i]);
		i++;
	}
	str[i] = 0;
	return (str);
}
