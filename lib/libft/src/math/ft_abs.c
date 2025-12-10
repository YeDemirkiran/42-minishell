/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_abs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 19:19:24 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:19:32 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

// Returns the absolute value of an integer.
unsigned int	ft_abs(int n)
{
	unsigned int	un;

	if (n < 0)
		un = (unsigned int)(-(long)(n));
	else
		un = (unsigned int)n;
	return (un);
}
