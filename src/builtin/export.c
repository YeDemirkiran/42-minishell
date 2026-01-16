/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:19:32 by muayna            #+#    #+#             */
/*   Updated: 2025/12/30 22:31:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

int export(t_envp_list **list, t_ast *ast)
{
	t_list *command;
	t_envp_list *copy_list;
	int return_value;

	return_value = 0;
	command = ast->data->ast_command.arguments;
	if(command->next == NULL)
	{
		copy_list = copy_envplist((*list));
		print_list(&copy_list);
	}
	else
	{
		command = command->next;
		return_value = add_variable_on_envplist(list, command);
	}
	return return_value;
}
