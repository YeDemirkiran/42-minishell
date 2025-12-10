/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:09:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 18:18:19 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

// Converts an uppercase character to lowercase and returns the converted char.
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
