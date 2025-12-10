/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:04:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 21:44:00 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libft/predicate.h"

// Converts an ASCII string to integer.
int	ft_atoi(const char *nptr)
{
	int	num;
	int	sign;

	num = 0;
	sign = 1;
	while (ft_is_whitespace(*nptr))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (ft_isdigit(*nptr))
		num = (num * 10) + (*(nptr++) - 48);
	return (num * sign);
}
