/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/28 16:00:54 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/31 19:12:01 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include <libft/string.h>
#include <libft/memory.h>

#include "structs/s_ast.h"

static void	set_quote(char c, char *quote)
{
	if (c == '\'' && *quote != '"')
	{
		if (*quote == '\'')
			*quote = 0;
		else
			*quote = '\'';
	}
	else if (c == '"' && *quote != '\'')
	{
		if (*quote == '"')
			*quote = 0;
		else
			*quote = '"';
	}
}

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

void	perform_quote_removal(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
		remove_quotes_args(ast->data->ast_command.arguments);
	if (ast->left != NULL)
		perform_quote_removal(ast->left);
	if (ast->right != NULL)
		perform_quote_removal(ast->right);
}
