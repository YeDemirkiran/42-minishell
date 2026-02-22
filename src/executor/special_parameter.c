/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_parameter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:49:34 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 15:21:25 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell_lb.h>
#include <structs/s_shell.h>

void	update_special_parameter(t_ast *ast, t_shell *shell)
{
	t_list	*args;

	if (ast->type != AST_COMMAND)
		return ;
	args = ast->data->ast_command.arguments;
	if (args == NULL)
		return ;
	while (args->next != NULL)
	{
		args = args->next;
	}
	update_envp_value(&shell->env->envp_list, "_", (char *)args->content);
}
