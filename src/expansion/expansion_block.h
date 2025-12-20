/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_block.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:54:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/20 21:02:54 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXPANSION_BLOCK_H
# define EXPANSION_BLOCK_H

# include <stddef.h>

typedef struct s_expansion_word
{
	char	*str;
	size_t	start;
	size_t	end;
}			t_expansion_word;

typedef union u_expansion
{
	char				*variable_key;
	t_expansion_word	word;
}			t_expansion;


#endif
