/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:23:09 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:22:25 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "libft/predicate.h"

// Checks if a char is alphanumeric.
int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
