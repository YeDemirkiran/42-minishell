/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:09:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:35:14 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

// Finds the first occurence of string 'little' in string 'big' if it exists
// within 'len' bytes.
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	int		i;
	size_t	len2;

	if (!*little)
		return ((char *) big);
	while (*big && len > 0)
	{
		i = 0;
		len2 = len--;
		while (little[i] && big[i] == little[i] && len2-- > 0)
			i++;
		if (little[i] == 0)
			return ((char *) big);
		big++;
	}
	return (NULL);
}
