/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_bonus.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:49:52 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/02 10:56:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef GLOB_BONUS_H
# define GLOB_BONUS_H

# include <libft/linked_list.h>

# define GLOB_BUFFER_SIZE 4096

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
 * @brief Initializes the given glob struct's flags and other parameters.
 */
void	set_glob_struct(const char *pattern, t_glob *glob);

/**
 * @brief Takes a pattern, matches the files in the current directory
 * and returns a struct of matched files' list.
 */
t_glob	ft_glob(const char *pattern);

#endif
