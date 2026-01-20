/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:48:53 by muayna            #+#    #+#             */
/*   Updated: 2026/01/20 14:10:15 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <structs/s_shell.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

void exit_builtin(t_ast *ast, t_shell *shell)
{
	unsigned char exit_value;
	t_list *args;

	args = ast->data->ast_command.arguments;
	if(args->next != NULL)
		exit_value = (unsigned char)(ft_atoi((char *)args->next->content));
	else
		exit_value = 0;
	shell_exit(exit_value, (*shell));
}