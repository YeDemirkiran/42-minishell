/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion_bonus.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:53:23 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/30 17:31:59 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef FILENAME_EXPANSION_BONUS_H
# define FILENAME_EXPANSION_BONUS_H

# include <libft/linked_list.h>

typedef enum e_glob_flags
{
	GLOB_FLAG_NONE = 0,
	GLOB_FLAG_EXPAND_HIDDEN = 1,
	GLOB_FLAG_EXPAND_DIRS = 2,
	GLOB_FLAG_ADD_LOCAL_PREFIX = 4,
}	t_glob_flags;

typedef enum e_glob_result
{
	GLOB_SUCCESS,
	GLOB_NO_NEED,
	GLOB_NO_MATCH,
	GLOB_FAILURE,
}	t_glob_result;

typedef struct s_glob
{
	t_glob_result	result;
	t_glob_flags	flags;
	t_list			*word_list;
}	t_glob;

/**
 * @brief Recursively performs filename expansion on a syntax tree,
 * if the node's type is AST_COMMAND.
 */
int	perform_filename_expansion(t_ast *ast);

#endif
