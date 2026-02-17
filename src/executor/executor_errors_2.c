/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_errors_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 19:28:54 by muayna            #+#    #+#             */
/*   Updated: 2026/02/17 19:29:03 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../../lib/garbage_collector/gb.h"
#include "enums/e_redir_type.h"
#include "fcntl.h"
#include "modules/ast.h"
#include "modules/builtin.h"
#include "modules/executor.h"
#include "modules/signal.h"
#include "modules/utils.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "sys/wait.h"
#include "unistd.h"
#include <errno.h>
#include <libft.h>
#include <minishell_lb.h>
#include <readline/readline.h>

void	print_exec_error_message(char **args, char **envp, int interactive)
{
	char	*not_found_args[3];

	if (errno == ENOENT || args[0][0] == '\0')
	{
		if (interactive == 1)
		{
			not_found_args[0] = "/usr/lib/command-not-found";
			not_found_args[1] = args[0];
			not_found_args[2] = NULL;
			execve(not_found_args[0], not_found_args, envp);
		}
		error_message("minishell", args[0], "command not found\n");
	}
	else
		error_message("minishell", args[0], NULL);
}