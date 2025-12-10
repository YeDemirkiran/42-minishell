/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_gb.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 17:00:14 by muayna            #+#    #+#             */
/*   Updated: 2025/11/16 03:03:10 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef S_GB_H
# define S_GB_H

typedef struct s_gb
{
	void		*address_pointer;
	struct s_gb	*next;
}	t_gb;

#endif
