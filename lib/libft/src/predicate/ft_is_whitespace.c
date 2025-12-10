/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_whitespace.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:40:17 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 21:05:29 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

// Checks if a character is whitespace (between 9 and 13, or a space).
int	ft_is_whitespace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}
