/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 10:45:53 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:20:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include <libft/string.h>
#include <libft/linked_list.h>

#include "glob_bonus.h"
#include "pattern_match_bonus.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Allocates the new filename on heap using the flags.
 *
 * @return NULL on failure.
 */
static char	*get_correct_file_name_flags(const char *original_name,
	t_glob_flags flags, mode_t st_mode)
{
	char	*new_name;
	char	*tmp;

	new_name = NULL;
	if (flags & GLOB_FLAG_ADD_LOCAL_PREFIX && original_name[0] != '.')
	{
		new_name = ft_strjoin("./", original_name);
		if (new_name == NULL)
			return (NULL);
	}
	if (flags & GLOB_FLAG_EXPAND_DIRS && S_ISDIR(st_mode))
	{
		if (new_name == NULL)
			new_name = ft_strjoin(original_name, "/");
		else
		{
			tmp = ft_strjoin(new_name, "/");
			free(new_name);
			new_name = tmp;
		}
	}
	if (new_name == NULL)
		new_name = ft_strdup(original_name);
	return (new_name);
}

/**
 * @brief Takes the original file name and returns the correct
 * name for pattern matching using the given flags. The returned
 * string is heap-allocated and should be free'd when no longer in use.
 *
 * @return NULL on failure.
 */
static char	*get_correct_file_name(const char *original_name,
	t_glob_flags flags)
{
	struct stat	buff;
	int			res;
	char		*new_name;

	new_name = ft_strdup(original_name);
	if (new_name == NULL)
		return (NULL);
	if (!(flags & GLOB_FLAG_EXPAND_DIRS)
		&& !(flags & GLOB_FLAG_ADD_LOCAL_PREFIX))
		return (new_name);
	res = stat(original_name, &buff);
	if (res < 0)
		return (new_name);
	free(new_name);
	new_name = get_correct_file_name_flags(original_name, flags, buff.st_mode);
	return (new_name);
}

static int	readdir_and_pattern_match(DIR *dir, t_glob *glob,
	const char *pattern, char **filename_addr)
{
	struct dirent	*dir_entry;

	dir_entry = readdir(dir);
	if (dir_entry == NULL)
	{
		if (errno != 0)
		{
			ft_lstclear(&(glob->word_list), free);
			glob->result = GLOB_FAILURE;
		}
		return (-1);
	}
	*filename_addr = get_correct_file_name(dir_entry->d_name, glob->flags);
	if (*filename_addr == NULL)
	{
		ft_lstclear(&(glob->word_list), free);
		glob->result = GLOB_FAILURE;
		return (-1);
	}
	if (pattern_match(pattern, *filename_addr, glob->flags) != 1)
	{
		free(*filename_addr);
		return (0);
	}
	return (1);
}

static int	ft_glob_add_to_list(const char *filename, t_glob *glob)
{
	t_list	*new;

	new = ft_lstnew((void *)(filename));
	if (new == NULL)
	{
		ft_lstclear(&(glob->word_list), free);
		glob->result = GLOB_FAILURE;
		return (0);
	}
	ft_lstadd_sorted(&(glob->word_list), new);
	return (1);
}

/**
 * @brief Takes a pattern, matches the files in the current directory
 * and returns a struct of matched files' list.
 */
t_glob	ft_glob(const char *pattern)
{
	t_glob			glob;
	DIR				*dir;
	int				result;
	char			*filename;

	set_glob_struct(pattern, &glob);
	if (glob.result != GLOB_SUCCESS)
		return (glob);
	dir = opendir(".");
	if (dir == NULL)
		return ((t_glob){.result = GLOB_FAILURE, .word_list = NULL});
	while (1)
	{
		errno = 0;
		result = readdir_and_pattern_match(dir, &glob, pattern, &filename);
		if (result == -1)
			break ;
		else if (result == 0)
			continue ;
		result = ft_glob_add_to_list(filename, &glob);
		if (result == 0)
			break ;
	}
	closedir(dir);
	return (glob);
}

/* ================================== END ================================== */
