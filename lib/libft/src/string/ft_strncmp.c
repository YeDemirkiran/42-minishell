/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:09:14 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:34:52 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

// Compares two strings until the n-th byte and returns their difference.
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (n-- > 0)
	{
		if ((!*str1 || !*str2) || (*str1 != *str2))
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	return (0);
}
