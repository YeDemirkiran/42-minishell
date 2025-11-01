/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_linked_list.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:52:48 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/20 23:55:07 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef S_LINKED_LIST_H
# define S_LINKED_LIST_H

typedef struct s_linked_list
{
	t_linked_list	*next;
	void			*content;
}			t_linked_list;

#endif
