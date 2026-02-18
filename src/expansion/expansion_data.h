/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_data.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:47:41 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:34:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_DATA_H
# define EXPANSION_DATA_H

# include <stddef.h>

# include <libft/linked_list.h>

# include "structs/s_shell.h"

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

/**
 * @brief Takes a word and outputs a linked list of expansion data blocks.
 *
 * These blocks are required for the second step of expansion: Concatenation.
 */
t_list	*get_expansion_data(char *word, t_shell shell, int ignore_quotes);

#endif
