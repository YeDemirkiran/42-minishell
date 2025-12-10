/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:09:01 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:27:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include "libft/string.h"

// Duplicates a string and returns the memory address of the copy.
char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*str;

	len = ft_strlen(s) + 1;
	str = malloc(len);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s, len);
	return (str);
}
