/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:49:51 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:42:50 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include "libft/string.h"

// Writes a string in a file.
void	ft_putstr_fd(char *s, int fd)
{
	size_t	len;

	len = ft_strlen(s);
	write(fd, s, len);
}
