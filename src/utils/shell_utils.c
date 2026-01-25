/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 07:52:25 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/24 03:29:29 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <readline/readline.h>
#include <libft/string.h>
#include <minishell_lb.h>
#include <gb.h>

#include "structs/s_shell.h"

/**
 * @brief Prints an optional exit messsage depending on 'interactive'
 * and exits with status.
 */
void	shell_exit(unsigned char status, t_shell shell)
{
	sort_malloc(1, 1);
	clear_arr(&(shell.env->envp_char));
	rl_clear_history();
	if (shell.interactive == 1)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
	}
	exit(status);
}

/**
 * @brief Empty function that does nothing, just returns 0. This is used
 * to trick GNU Readline's rl_event_hook so it stops periodically to check
 * rl_done variable's value and exits.
 * @return 0
 */
int	rl_event(void)
{
	return (0);
}
