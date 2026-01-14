/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 16:00:54 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/14 15:38:52 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include <libft/string.h>
#include <libft/memory.h>

#include "structs/s_ast.h"
#include "enums/e_redir_type.h"
#include "structs/s_redirection.h"
#include "modules/utils.h"

/**
 * @brief Takes a string and removes outer-level quotes from it.
 * @note Quotes inside quotes are preserved.
 */
void	remove_quotes(char *str)
{
	size_t	i;
	char	quote;
	char	c;

	if (str == NULL)
		return ;
	i = 0;
	quote = 0;
	while (str[i] != '\0')
	{
		c = str[i];
		if (quote == c || (quote == 0 && (c == '\'' || c == '"')))
		{
			set_quote(c, &quote);
			ft_memmove(str + i, str + i + 1, ft_strlen(str + i + 1) + 1);
			continue ;
		}
		i++;
	}
}

static void	remove_quotes_args(t_list *args)
{
	if (args == NULL)
		return ;
	while (args != NULL)
	{
		if (args->content != NULL)
			remove_quotes((char *)(args->content));
		args = args->next;
	}
}

static void	remove_quotes_redirs(t_list *redirs)
{
	t_redirection	*redir;

	if (redirs == NULL)
		return ;
	while (redirs != NULL)
	{
		redir = (t_redirection *)(redirs->content);
		if (redir != NULL && redir->redir_type != REDIR_HEREDOC)
			remove_quotes(redir->path);
		redirs = redirs->next;
	}
}

void	perform_quote_removal(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
	{
		remove_quotes_args(ast->data->ast_command.arguments);
		remove_quotes_redirs(ast->data->ast_command.redirs);
	}
	if (ast->left != NULL)
		perform_quote_removal(ast->left);
	if (ast->right != NULL)
		perform_quote_removal(ast->right);
}
