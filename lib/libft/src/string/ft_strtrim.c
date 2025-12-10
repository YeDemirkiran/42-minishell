/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:50:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:35:34 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libft/string.h"

// Trims each character from 'set' from the start and end of a string.
char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	char	*str;
	size_t	start_index;
	size_t	end_index;

	len = ft_strlen(s1);
	start_index = 0;
	while (s1[start_index] && ft_strchr(set, s1[start_index]))
		start_index++;
	end_index = len - 1;
	while (ft_strchr(set, s1[end_index]) && end_index > 0)
		end_index--;
	if (end_index >= start_index)
		len = end_index - start_index + 1;
	else
		len = 0;
	str = ft_substr(s1, start_index, len);
	if (str == NULL)
		return (NULL);
	return (str);
}
