/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:07:51 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:21:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libft/memory.h"

// Same as ft_memcpy but overlap safe.
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*tmp_src;
	unsigned char	*tmp_dest;

	tmp_src = (unsigned char *)src;
	tmp_dest = (unsigned char *)dest;
	if (tmp_dest > tmp_src && tmp_src + n > tmp_dest)
	{
		while (n-- > 0)
			tmp_dest[n] = tmp_src[n];
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}
