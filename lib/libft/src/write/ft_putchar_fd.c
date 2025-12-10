/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:49:37 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 20:37:15 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>

// Writes char c in a file.
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
