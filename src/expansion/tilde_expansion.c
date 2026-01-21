/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 21:50:43 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/16 12:13:15 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <errno.h>

#include <libft/string.h>
#include <minishell_lb.h>

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "structs/s_shell.h"

#include "variable_utils.h"

static char	*get_tilde_expanded_string(const char *str, const char *home)
{
	char	*path_start;
	char	*word;

	if (home == NULL || str == NULL || *str != '~')
		return (NULL);
	path_start = ft_strchr(str, '/');
	if (path_start == NULL)
	{
		if (str[1] == '\0')
			return (ft_strdup(home));
		return (NULL);
	}
	path_start = ft_strdup(path_start);
	if (path_start == NULL)
		return (NULL);
	word = ft_strjoin(home, path_start);
	free(path_start);
	return (word);
}

static int	expand_tilde_word_list(t_list *node, const char *home)
{
	char	*word;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		if (node->content == NULL)
		{
			node = node->next;
			continue ;
		}
		word = get_tilde_expanded_string((char *)(node->content), home);
		if (word != NULL)
		{
			free(node->content);
			node->content = (void *)word;
		}
		else if (errno != 0)
			return (0);
		node = node->next;
	}
	return (1);
}

static int	expand_tilde_redir_list(t_list *node, const char *home)
{
	char			*word;
	t_redirection	*redir;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		if (node->content == NULL)
		{
			node = node->next;
			continue ;
		}
		redir = (t_redirection *)node->content;
		word = get_tilde_expanded_string((char *)(redir->path), home);
		if (word != NULL)
		{
			free(redir->path);
			redir->path = word;
		}
		else if (errno != 0)
			return (0);
		node = node->next;
	}
	return (1);
}

static int	tilde_expand_command(t_ast *ast, const char *home)
{
	t_list	*args;
	t_list	*redirs;

	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
		return (2);
	args = ast->data->ast_command.arguments;
	if (args != NULL)
		if (expand_tilde_word_list(args, home) != 1)
			return (0);
	redirs = ast->data->ast_command.redirs;
	if (redirs != NULL)
		if (expand_tilde_redir_list(redirs, home) != 1)
			return (0);
	return (1);
}

static int	tilde_expansion_recursive(t_ast *root, const char *home)
{
	if (root->type == AST_COMMAND)
	{
		if (tilde_expand_command(root, home) != 1)
			return (0);
	}
	if (root->left != NULL)
	{
		if (tilde_expansion_recursive(root->left, home) != 1)
			return (0);
	}
	if (root->right != NULL)
	{
		if (tilde_expansion_recursive(root->right, home) != 1)
			return (0);
	}
	return (1);
}

/**
 * @brief Takes a syntax tree and expands tilde symbols in it.
 * @return 0 on failure, 1 on success (or if HOME is unset and there is nothing
 * to do), 2 on NULL root parameter.
 */
int	perform_tilde_expansion(t_ast *root, t_shell shell)
{
	char	*home;
	int		result;

	if (root == NULL)
		return (2);
	home = get_env_var("HOME", shell);
	if (home == NULL)
		return (1);
	result = tilde_expansion_recursive(root, home);
	free(home);
	return (result == 1);
}
