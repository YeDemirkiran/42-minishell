/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:59:40 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:20:22 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libft/memory.h"

// Same as Unix bzero. Overwrites a memory block of size n with zeroes.
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
