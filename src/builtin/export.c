/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:19:32 by muayna            #+#    #+#             */
/*   Updated: 2025/12/29 19:19:32 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

void export(t_envp_list **list, t_ast *ast)
{
	t_envp_list *tmp;

	tmp = (*list);
	if(ast->data->ast_command.arguments->next == NULL)
	{
		printf("DEBUG EXPORT : SUCCES\n");
		return ;
	}
	while(tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = ft_malloc(sizeof(t_envp_list), 0);
	tmp = tmp->next;
	tmp->id_name = ft_strdup((char *)ast->data->ast_command.arguments->next->content);
	tmp->value = ft_strdup((char *)ast->data->ast_command.arguments->next->next->content);
	printf("%s%s\n", tmp->id_name, tmp->value);
	tmp->next = NULL;
	return ;
}