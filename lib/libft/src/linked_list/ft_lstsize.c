/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:49:22 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:19:13 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include "libft/linked_list.h"

// Returns the size of a linked list.
int	ft_lstsize(t_list *lst)
{
	int	size;

	if (lst == NULL)
		return (0);
	size = 0;
	while ((*lst).next)
	{
		size++;
		lst = lst->next;
	}
	return (size + 1);
}
