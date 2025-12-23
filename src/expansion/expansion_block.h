/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_block.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:54:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/22 06:27:21 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXPANSION_BLOCK_H
# define EXPANSION_BLOCK_H

# include <stddef.h>

typedef enum e_expansion_type
{
	EXPANSION_WORD,
	EXPANSION_VARIABLE
}	t_expansion_type;

typedef union u_expansion_data
{
	char	*variable_value;
	struct s_expansion_word
	{
		size_t	start;
		size_t	end;
	}		word;
}	t_expansion_data;

typedef struct s_expansion
{
	t_expansion_type	type;
	t_expansion_data	data;
}			t_expansion;

#endif
