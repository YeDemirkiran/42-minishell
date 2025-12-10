/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:59:15 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:21:29 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

// Copies the first n bytes from src to dest.
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*tmp_dest;
	unsigned char	*tmp_src;

	tmp_src = (unsigned char *)src;
	tmp_dest = (unsigned char *)dest;
	while (n-- > 0)
		*(tmp_dest++) = *(tmp_src++);
	return (dest);
}
