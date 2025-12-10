/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:48:50 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 19:18:26 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include "libft/linked_list.h"

// Adds a node to the end of a linked list.
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (new == NULL || lst == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
}
