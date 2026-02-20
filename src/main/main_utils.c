/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:12:18 by muayna            #+#    #+#             */
/*   Updated: 2026/02/20 20:54:27 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include <readline/readline.h>

#include "modules/utils.h"

#include "args.h"
#include "shell_init.h"

void	init_main(t_shell *shell, int argc, char **argv, char **envp)
{
	int	init_result;

	if (process_args((const char **)argv, argc) == 0)
		exit(EXIT_SUCCESS);
	init_result = init_shell(shell, argc, argv, envp);
	if (init_result != 0)
		shell_exit(init_result, *shell);
	rl_catch_signals = 0;
	rl_event_hook = rl_event;
}
