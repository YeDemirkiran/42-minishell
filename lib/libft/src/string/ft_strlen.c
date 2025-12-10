/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:59:51 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/20 16:31:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>

// Returns the size of a string.
size_t	ft_strlen(const char *c)
{
	size_t	count;

	if (c == NULL)
		return (0);
	count = 0;
	while (c[count])
		count++;
	return (count);
}
