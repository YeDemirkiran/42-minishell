/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:09:04 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 21:56:12 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libft/string.h"

// Same as glibc strlcat.
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	total_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size <= dst_len)
		return (size + src_len);
	total_len = dst_len + src_len;
	ft_strlcpy(dst + dst_len, src, size - dst_len);
	return (total_len);
}
