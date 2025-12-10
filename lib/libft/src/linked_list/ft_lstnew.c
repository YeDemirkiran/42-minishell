/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:49:16 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/16 21:10:05 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include "libft/linked_list.h"

// Creates and retuns a new linked list node.
t_list	*ft_lstnew(void *content)
{
	t_list	*item;

	item = malloc(sizeof(t_list));
	if (!item)
		return (NULL);
	(*item).content = content;
	(*item).next = NULL;
	return (item);
}
