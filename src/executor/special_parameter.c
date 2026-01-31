/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_parameter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:49:34 by muayna            #+#    #+#             */
/*   Updated: 2026/01/29 17:12:52 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "stdio.h"
#include <libft.h>
#include <errno.h>
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"
#include "sys/wait.h"
#include "signal.h"
#include "../../lib/garbage_collector/gb.h"

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"
#include <minishell_lb.h>
#include "modules/executor.h"
#include "modules/builtin.h"
#include "modules/signal.h"
#include "modules/utils.h"

void	update_special_parameter(t_ast *ast, t_shell *shell)
{
    t_list *args;

    if(ast->type != AST_COMMAND)
        return ;
    args = ast->data->ast_command.arguments;
    while(args->next != NULL)
    {
        args = args->next;
    }
    update_envp_value(&shell->env->envp_list, "_", (char *)args->content);
}