/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:08:34 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:54 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

// Returns the address of the first instance of 'c' in a memory block.
void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	uc;

	str = (unsigned char *)s;
	uc = (unsigned char)c;
	while (n-- > 0)
	{
		if (*str == uc)
			return ((void *)str);
		str++;
	}
	return (NULL);
}
