/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:00:11 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/16 11:11:55 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <libft/string.h>
#include <libft/linked_list.h>

#include "structs/s_ast.h"
#include "modules/utils.h"

#include "variable_utils.h"

static char	*get_ifs(t_shell shell)
{
	static const char	*default_ifs = " \t\n";
	char				*ifs;

	ifs = get_env_var("IFS", shell);
	if (ifs == NULL)
		ifs = ft_strdup(default_ifs);
	return (ifs);
}

static int	save_split_word(char *str, int i, t_list **root)
{
	char	*str_tmp;
	t_list	*node;

	if (str == NULL || root == NULL)
		return (2);
	str_tmp = ft_substr(str, 0, i);
	if (str_tmp == NULL)
	{
		ft_lstclear(root, free);
		return (0);
	}
	node = ft_lstnew((void *)str_tmp);
	if (node == NULL)
	{
		free(str_tmp);
		ft_lstclear(root, free);
		return (0);
	}
	ft_lstadd_back(root, node);
	return (1);
}

/**
 * @brief Splits a string into words according to the IFS variable,
 * and returns it in linked list form.
 * @note IFS is retrieved by getenv. If it doesn't exist, then the default
 * IFS is used.
 * @note The defualt IFS is (space)(tab)(newline).
 * @note If IFS is set but has no characters, word splitting is disabled.
 */
static t_list	*split_words(char *str, const char *ifs)
{
	t_list	*root;
	size_t	i;
	char	q;

	if (ft_strlen(ifs) == 0)
		return (NULL);
	i = 0;
	root = NULL;
	q = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && ft_strchr(ifs, str[i]) != NULL)
			i++;
		if (str[i] == '\0')
			break ;
		str += i;
		i = 0;
		while (str[i] != '\0' && (q != 0 || ft_strchr(ifs, str[i]) == NULL))
			set_quote(str[i++], &q);
		if (save_split_word(str, i, &root) == 0)
			return (NULL);
	}
	return (root);
}

static int	does_need_split(char *str, const char *ifs)
{
	size_t	i;
	char	quote;

	if (ft_strlen(ifs) == 0)
		return (0);
	i = 0;
	quote = 0;
	while (str[i] != '\0')
	{
		set_quote(str[i], &quote);
		if (str[i] != '\0' && (quote == 0 && ft_strchr(ifs, str[i]) != NULL))
			return (1);
		i++;
	}
	return (0);
}

static int	split_word_advance_loop(t_list **args,
	t_list **node, const char *ifs)
{
	t_list	*tmp;

	if (does_need_split((char *)((*node)->content), ifs) != 1)
	{
		*node = (*node)->next;
		return (1);
	}
	tmp = split_words((char *)((*node)->content), ifs);
	if (errno != 0)
	{
		perror("minishell");
		return (0);
	}
	else if (tmp != NULL)
	{
		ft_lst_replace(args, *node, tmp, free);
		*node = tmp->next;
	}
	else
	{
		tmp = (*node)->next;
		ft_lstdel_node(args, *node, free);
		*node = tmp;
	}
	return (1);
}

/**
 * @brief Takes a list of arguments, and performs word splitting
 * on all of them.
 * @return 0 on failure, 1 on success, 2 on NULL parameters.
 */
static int	split_word_args(t_list **args, const char *ifs)
{
	t_list	*node;
	t_list	*tmp;

	if (args == NULL || *args == NULL)
		return (2);
	node = *args;
	while (node != NULL)
	{
		if (node->content == NULL)
		{
			tmp = node->next;
			ft_lstdel_node(args, node, NULL);
			node = tmp;
		}
		else
		{
			if (split_word_advance_loop(args, &node, ifs) != 1)
				return (0);
		}
	}
	return (1);
}

static int	word_split_recursive(t_ast *ast, const char *ifs)
{
	if (ast->type == AST_COMMAND)
	{
		if (split_word_args(&(ast->data->ast_command.arguments), ifs) == 0)
			return (0);
	}
	if (ast->left != NULL)
	{
		if (word_split_recursive(ast->left, ifs) != 1)
			return (0);
	}
	if (ast->right != NULL)
	{
		if (word_split_recursive(ast->right, ifs) != 1)
			return (0);
	}
	return (1);
}

/**
 * @brief Takes a syntax tree and recursively splits words in their arguments.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	perform_word_split(t_ast *ast, t_shell shell)
{
	char	*ifs;
	int		result;

	if (ast == NULL)
		return (2);
	ifs = get_ifs(shell);
	if (ifs == NULL)
		return (0);
	result = word_split_recursive(ast, ifs);
	free(ifs);
	return (result);
}
