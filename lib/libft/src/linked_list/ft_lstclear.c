/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:48:59 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:18:46 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include "libft/linked_list.h"

// Clears a linked list from start to end.
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (lst == NULL || *lst == NULL)
		return ;
	tmp = *lst;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
}
