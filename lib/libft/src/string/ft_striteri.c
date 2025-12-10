/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:50:03 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 18:43:10 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

// Iterates through a string and applies function 'f' on each character.
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		(*f)(i, s + i);
		i++;
	}
}
