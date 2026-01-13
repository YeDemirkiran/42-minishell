/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 21:50:43 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 15:58:31 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <errno.h>

#include <libft/string.h>
#include <minishell_lb.h>

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "structs/s_shell.h"

static char	*get_tilde_expanded_string(const char *str, t_envp_list **list)
{
	char	*home;
	char	*path_start;
	char	*word;

	if (str == NULL || *str != '~')
		return (NULL);
	home = find_value_on_envp(list, "HOME");
	if (home == NULL)
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

static int	expand_tilde_word_list(t_list *node, t_envp_list **list)
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
		word = get_tilde_expanded_string((char *)(node->content), list);
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

static int	expand_tilde_redir_list(t_list *node, t_envp_list **list)
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
		word = get_tilde_expanded_string((char *)(redir->path), list);
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

static int	tilde_expand_command(t_ast *ast, t_envp_list **list)
{
	t_list	*args;
	t_list	*redirs;

	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
		return (2);
	args = ast->data->ast_command.arguments;
	if (args != NULL)
		if (expand_tilde_word_list(args, list) != 1)
			return (0);
	redirs = ast->data->ast_command.redirs;
	if (redirs != NULL)
		if (expand_tilde_redir_list(redirs, list) != 1)
			return (0);
	return (1);
}

/**
 * @brief Takes a syntax tree and expands tilde symbols in it.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	perform_tilde_expansion(t_ast *root, t_envp_list **list)
{
	if (root == NULL)
		return (2);
	if (root->type == AST_COMMAND)
	{
		if (tilde_expand_command(root, list) != 1)
			return (0);
	}
	if (root->left != NULL)
	{
		if (perform_tilde_expansion(root->left, list) != 1)
			return (0);
	}
	if (root->right != NULL)
	{
		if (perform_tilde_expansion(root->right, list) != 1)
			return (0);
	}
	return (1);
}
