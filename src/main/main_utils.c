/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 12:01:22 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 12:01:23 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
