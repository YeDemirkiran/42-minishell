/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:17:24 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:41 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include "libft/memory.h"

// Same as glibc calloc.
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;
	size_t	c_size;

	if (!nmemb || !size)
		return (malloc(1));
	else if (nmemb > (size_t)-1 / size)
		return (NULL);
	c_size = nmemb * size;
	arr = malloc(c_size);
	if (!arr)
		return (NULL);
	ft_bzero(arr, c_size);
	return (arr);
}
