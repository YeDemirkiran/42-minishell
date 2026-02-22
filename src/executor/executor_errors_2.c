/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_errors_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 19:28:54 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 14:48:40 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>

#include <stdlib.h>
#include <unistd.h>

#include "modules/utils.h"

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
