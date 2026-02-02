/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:53:23 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/02 09:09:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include "modules/utils.h"

#include "filename_expansion_bonus.h"
#include "remove_quotes.h"

#define GLOB_BUFFER_SIZE 4096

/**
 * @brief Initializes the given glob struct's flags and other parameters.
 */
static void	set_glob_struct(const char *pattern, t_glob *glob)
{
	size_t	i;
	char	c;
	char	quote;

	if (pattern == NULL || glob == NULL)
		return ;
	i = 0;
	quote = 0;
	glob->result = GLOB_NO_NEED;
	glob->flags = GLOB_FLAG_NONE;
	glob->word_list = NULL;
	while (pattern[i] != '\0')
	{
		c = pattern[i];
		if (i == 0 && c == '.' && pattern[1] == '/')
			glob->flags |= GLOB_FLAG_ADD_LOCAL_PREFIX;
		else if (i == 0 && c == '.')
			glob->flags |= GLOB_FLAG_EXPAND_HIDDEN;
		set_quote(c, &quote);
		if (c == '*' && quote == 0)
			glob->result = GLOB_SUCCESS;
		i++;
	}
	if (i > 0 && pattern[i - 1] == '/')
		glob->flags |= GLOB_FLAG_EXPAND_DIRS;
	if (i >= GLOB_BUFFER_SIZE)
		glob->result = GLOB_NO_NEED;
}

/**
 * @brief Takes a buffer and a word, and tries to find it in
 * the given name.
 *
 * If first_occurrence is positive, the first occurrence of the buffer is
 * returned. Otherwise, the last occurrence is returned.
 */
static char	*find_buffer_in_name(const char *name,
	const char *buff, int first_occurrence)
{
	const char	*previous;
	const char	*current;

	if (name == NULL || buff == NULL)
		return (NULL);
	previous = NULL;
	current = name;
	while (*current != '\0')
	{
		current = ft_strnstr(current, buff, ft_strlen(current));
		if (current == NULL)
			break ;
		else if (first_occurrence > 0)
			return ((char *)(current));
		previous = current;
		current = current + 1;
	}
	return ((char *)previous);
}

static int	pattern_match_initial_check(const char *pattern, const char *name,
	t_glob_flags flags)
{
	int	expand_hidden;
	int	add_local_prefix;

	if (pattern == NULL || name == NULL)
		return (0);
	expand_hidden = (flags & GLOB_FLAG_EXPAND_HIDDEN);
	add_local_prefix = (flags & GLOB_FLAG_ADD_LOCAL_PREFIX);
	if (!expand_hidden && !add_local_prefix && name[0] == '.')
		return (0);
	return (1);
}

static void	pattern_match_prepare_buffer(char *buffer, const char *pattern,
	size_t *i, size_t buffer_start)
{
	char	quote;

	quote = 0;
	while ((quote != 0 || pattern[*i] != '*') && pattern[*i] != '\0')
		set_quote(pattern[(*i)++], &quote);
	ft_strlcpy(buffer, pattern + buffer_start, *i - buffer_start + 1);
	remove_quotes(buffer);
}

/**
 * @brief Takes a pattern and a name, and checks if they do match.
 *
 * @return 1 if pattern matches, 0 otherwise.
 */
static int	pattern_match(const char *pattern,
	const char *name, t_glob_flags flags)
{
	size_t	i;
	size_t	buffer_start;
	char	*current;
	char	buffer[GLOB_BUFFER_SIZE];

	if (pattern_match_initial_check(pattern, name, flags) != 1)
		return (0);
	i = 0;
	current = (char *)name;
	while (1)
	{
		while (pattern[i] == '*')
			i++;
		if (pattern[i] == '\0')
			break ;
		buffer_start = i;
		pattern_match_prepare_buffer(buffer, pattern, &i, buffer_start);
		current = find_buffer_in_name(current, buffer, buffer_start == 0);
		if (current == NULL || (buffer_start == 0 && current != name))
			return (0);
		else if (pattern[i] == '\0' && current[ft_strlen(buffer)] != '\0')
			return (0);
		current += i - buffer_start;
	}
	return (1);
}

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

/**
 * @brief Takes a pattern, matches the files in the current directory
 * and returns a struct of matched files' list.
 */
t_glob	ft_glob(const char *pattern)
{
	t_glob			glob;
	DIR				*dir;
	struct dirent	*dir_entry;
	t_list			*new;
	char			*tmp;
	char			*tmp_2;

	if (pattern == NULL)
		return ((t_glob){.result = GLOB_NO_NEED, .word_list = NULL});
	set_glob_struct(pattern, &glob);
	if (glob.result != GLOB_SUCCESS)
		return (glob);
	dir = opendir(".");
	if (dir == NULL)
		return ((t_glob){.result = GLOB_FAILURE, .word_list = NULL});
	while (1)
	{
		dir_entry = readdir(dir);
		if (dir_entry == NULL)
		{
			if (errno != 0)
			{
				ft_lstclear(&(glob.word_list), free);
				glob.result = GLOB_FAILURE;
			}
			break ;
		}
		tmp = get_correct_file_name(dir_entry->d_name, glob.flags);
		if (tmp == NULL)
		{
			ft_lstclear(&(glob.word_list), free);
			glob.result = GLOB_FAILURE;
			break ;
		}
		if (pattern_match(pattern, tmp, glob.flags) != 1)
		{
			free(tmp);
			continue ;
		}
		tmp_2 = ft_strdup(tmp);
		if (tmp_2 == NULL)
		{
			ft_lstclear(&(glob.word_list), free);
			glob.result = GLOB_FAILURE;
			break ;
		}
		free(tmp);
		new = ft_lstnew((void *)(tmp_2));
		if (new == NULL)
		{
			ft_lstclear(&(glob.word_list), free);
			glob.result = GLOB_FAILURE;
			break ;
		}
		ft_lstadd_sorted(&(glob.word_list), new);
	}
	closedir(dir);
	return (glob);
}

static void	filename_replace_arg(t_list **args, t_list **current,
	t_list *word_list)
{
	t_list	*next;

	next = (*current)->next;
	ft_lst_replace(args, *current, word_list, free);
	*current = next;
}

/**
 * @brief Takes a list of arguments, expands filenames in it
 * and then replaces the old word with the new word(list).
 */
static int	filename_expand_args(t_list **args)
{
	char	*str;
	t_glob	glob;
	t_list	*current;

	if (args == NULL || *args == NULL)
		return (2);
	current = *args;
	while (current != NULL)
	{
		if (current->content == NULL)
		{
			current = current->next;
			continue ;
		}
		str = (char *)(current->content);
		glob = ft_glob(str);
		if (glob.result == GLOB_SUCCESS)
			filename_replace_arg(args, &current, glob.word_list);
		else if (glob.result == GLOB_FAILURE)
			return (0);
		else
			current = current->next;
	}
	return (1);
}

/**
 * @brief Recursively performs filename expansion on a syntax tree,
 * if the node's type is AST_COMMAND.
 */
int	perform_filename_expansion(t_ast *ast)
{
	if (ast == NULL)
		return (2);
	if (ast->type == AST_COMMAND)
		if (filename_expand_args(&(ast->data->ast_command.arguments)) != 1)
			return (0);
	if (ast->left != NULL)
		if (perform_filename_expansion(ast->left) != 1)
			return (0);
	if (ast->right != NULL)
		if (perform_filename_expansion(ast->right) != 1)
			return (0);
	return (1);
}
