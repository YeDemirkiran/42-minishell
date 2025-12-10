/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:59:08 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:22:04 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

// Fills the first n bytes of a memory block with int c.
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*temp_char;

	temp_char = (unsigned char *)s;
	while (n-- > 0)
		*(temp_char++) = (unsigned char)c;
	return (s);
}
