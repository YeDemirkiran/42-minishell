/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 21:29:10 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/07 21:34:47 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <libft/write.h>

#include "structs/s_shell.h"

/**
 * @brief Prints out an error message with perror (or custom error message if present)
 * in this format:
 *
 * shell_name: [optional_field: ] perror() message
 */
void	shell_error_message(t_shell shell,
	char *extra_field, char *custom_error)
{
	char	*shell_name;

	shell_name = "minishell";
	if (shell.args != NULL && shell.args[0] != NULL)
		shell_name = shell.args[0];
	ft_putstr_fd(shell_name, STDERR_FILENO);
	if (extra_field != NULL)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(extra_field, STDERR_FILENO);
	}
	if (custom_error != NULL)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(custom_error, STDERR_FILENO);
	}
	else if (errno != 0)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(NULL);
	}
}

/**
 * @brief Converts the current errno value to shell error codes
 * and returns it. These inclue:
 *
 * - 0 - No error
 *
 * - 1 - Generic error
 *
 * - 126 - Permission denied, Is a directory
 *
 * - 126 - Is a directory
 *
 * - 127 - File or directory not found
 *
 * - 130 - SIGINT
 */
int	errno_to_exit_code(void)
{
	int	code;

	code = 0;
	if (errno == EACCES || errno == EISDIR)
		code = 126;
	else if (errno == ENOENT)
		code = 127;
	else if (EINTR)
		code = 128 + 2;
	else if (errno != 0)
		code = 1;
	return (code);
}
