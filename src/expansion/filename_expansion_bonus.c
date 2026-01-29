/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:53:23 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/29 15:09:53 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <dirent.h>

#include "modules/utils.h"

#include "filename_expansion_bonus.h"
#include "remove_quotes.h"

#define GLOB_BUFFER_SIZE 4096

static void	set_glob_struct(const char *pattern, t_glob *glob)
{
	size_t	i;
	char	c;
	char	quote;

	if (pattern == NULL || glob == NULL)
		return ;
	i = 0;
	quote = 0;
	while (pattern[i] != '\0')
	{
		c = pattern[i];
		if (i == 0 && c == '.')
			glob->flags |= GLOB_FLAG_EXPAND_HIDDEN;
		set_quote(c, &quote);
		if (c == '*' && quote == 0)
			glob->result = GLOB_SUCCESS;
		i++;
	}
	if (i >= GLOB_BUFFER_SIZE)
		glob->result = GLOB_NO_NEED;
}

/**
 * @brief Takes a list with char * content, and adds the new node
 * to its correct place.
 */
static void	lst_add_to_sorted_place(t_list **lst, t_list *new)
{
	t_list	*i;
	t_list	*pre;
	char	*str_1;
	char	*str_new;

	if (lst == NULL || new == NULL || new->content == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	str_new = (char *)(new->content);
	i = *lst;
	pre = NULL;
	while (i != NULL)
	{
		str_1 = (char *)(i->content);
		if (ft_strcmp(str_1, str_new) >= 0)
		{
			if (pre == NULL)
				ft_lstadd_front(lst, new);
			else
			{
				pre->next = new;
				new->next = i;
			}
			return ;
		}
		pre = i;
		i = i->next;
	}
	pre->next = new;
}

static int	pattern_match(const char *pattern,
	const char *name, t_glob_flags flags)
{
	size_t	j;
	size_t	k;
	char	*current;
	char	quote;
	char	buff[GLOB_BUFFER_SIZE];

	if (pattern == name)
		return (1);
	else if (pattern == NULL || name == NULL)
		return (0);
	if (!(flags & GLOB_FLAG_EXPAND_HIDDEN) && name[0] == '.')
		return (0);
	j = 0;
	quote = 0;
	current = (char *)name;
	while (1)
	{
		while (pattern[j] == '*')
			j++;
		if (pattern[j] == '\0')
			break ;
		k = j;
		while ((quote != 0 || pattern[j] != '*') && pattern[j] != '\0')
			set_quote(pattern[j++], &quote);
		ft_strlcpy(buff, pattern + k, j - k + 1);
		remove_quotes(buff);
		current = ft_strnstr(current, buff, ft_strlen(current));
		if (current == NULL || (k == 0 && current != name))
			return (0);
		else if (pattern[j] == '\0' && current[ft_strlen(buff)] != '\0')
			return (0);
		current += j - k;
	}
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
	struct dirent	*dir_entry;
	t_list			*new;

	glob.result = GLOB_NO_NEED;
	glob.flags = GLOB_FLAG_NONE;
	glob.word_list = NULL;
	if (pattern == NULL)
		return (glob);
	set_glob_struct(pattern, &glob);
	if (glob.result == GLOB_NO_NEED)
		return (glob);
	dir = opendir(".");
	while (1)
	{
		dir_entry = readdir(dir);
		if (dir_entry == NULL)
			break ;
		if (pattern_match(pattern, dir_entry->d_name, glob.flags) != 1)
			continue ;
		new = ft_lstnew((void *)(ft_strdup(dir_entry->d_name)));
		lst_add_to_sorted_place(&(glob.word_list), new);
	}
	closedir(dir);
	return (glob);
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
	t_list	*tmp;

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
		{
			tmp = current->next;
			ft_lst_replace(args, current, glob.word_list, free);
			current = tmp;
		}
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
		filename_expand_args(&(ast->data->ast_command.arguments));
	if (ast->left != NULL)
		perform_filename_expansion(ast->left);
	if (ast->right != NULL)
		perform_filename_expansion(ast->right);
	return (1);
}
