/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:53:23 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/25 23:40:03 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "modules/utils.h"

#include "filename_expansion_bonus.h"

static void	set_glob_struct(const char *pattern, t_glob *glob)
{
	size_t	i;
	char	c;
	//char	quote;

	if (pattern == NULL || glob == NULL)
		return ;
	i = 0;
	//quote = 0;
	while (pattern[i] != '\0')
	{
		c = pattern[i];
		if (i == 0 && c == '.')
			glob->flags |= GLOB_FLAG_EXPAND_HIDDEN;
		//set_quote(c, &quote);
		//if (c == '*' && quote == 0)
		if (c == '*')
			glob->result = GLOB_SUCCESS;
		i++;
	}
}

/**
 * @brief Takes a pattern, matches the files in the current directory
 * and returns a struct.
 */
t_glob	ft_glob(const char *pattern)
{
	t_glob	glob;

	glob.result = GLOB_NO_NEED;
	glob.flags = GLOB_FLAG_NONE;
	glob.word_list = NULL;
	if (pattern == NULL)
		return (glob);
	set_glob_struct(pattern, &glob);
	if (glob.result == GLOB_NO_NEED)
		return (glob);
	printf("yo this word needs globbing: %s\n", pattern);
	return (glob);
}

static int	filename_expand_args(t_list *args)
{
	char	*str;

	if (args == NULL)
		return (2);
	while (args != NULL)
	{
		if (args->content == NULL)
		{
			args = args->next;
			continue ;
		}
		str = (char *)(args->content);
		ft_glob(str);
		args = args->next;
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
	{
		filename_expand_args(ast->data->ast_command.arguments);
	}
	if (ast->left != NULL)
		perform_filename_expansion(ast->left);
	if (ast->right != NULL)
		perform_filename_expansion(ast->right);
	return (1);
}
