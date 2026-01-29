/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_parameter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:49:34 by muayna            #+#    #+#             */
/*   Updated: 2026/01/29 16:16:55 by muayna           ###   ########.fr       */
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

static int check_any_pipe(t_ast *ast)
{
    if((ast->left == NULL && ast->right == NULL) || ast->type != AST_PIPE)
        return 0;
    return 1;
}

void	update_special_parameter(t_ast *ast, t_shell *shell)
{
    t_list *args;

    args = ast->data->ast_command.arguments;
    if(check_any_pipe(ast))
        return ;
    while(args->next != NULL)
    {
        args = args->next;
    }
    update_envp_value(&shell->env->envp_list, "_", args->content);
}