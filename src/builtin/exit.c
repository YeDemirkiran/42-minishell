/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:48:53 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 07:10:20 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "structs/s_ast.h"
#include "unistd.h"
#include <dirent.h>
#include <libft.h>
#include <minishell_lb.h>
#include <modules/ast.h>
#include <modules/builtin.h>
#include <modules/utils.h>
#include <structs/s_shell.h>

static int	all_character_is_digit(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	if (str[i] == '-' || str[i] == '+')
	{
		i++;
		if (str[i] == '\0')
			return (1);
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	exit_builtin(t_ast *ast, t_shell *shell, int shell_interactive)
{
	unsigned char	exit_value;
	t_list			*args;

	args = ast->data->ast_command.arguments;
	if (args->next != NULL)
		exit_value = (unsigned char)(ft_atoi((char *)args->next->content));
	if (args->next == NULL)
		exit_value = 0;
	else if (all_character_is_digit((char *)args->next->content))
	{
		write(2, "babashell: exit: ", 18);
		write(2, args->next->content, ft_strlen((char *)args->next->content));
		write(2, ": numeric argument required\n", 29);
		exit_value = 2;
	}
	else if (ft_lstsize(args) > 2)
	{
		write(2, "babashell: exit: too many arguments\n", 37);
		shell->exit_status = 1;
		return ;
	}
	shell->interactive = shell_interactive;
	ast_free_tree(shell->root_ast);
	shell_exit(exit_value, (*shell));
}
