/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:12:18 by muayna            #+#    #+#             */
/*   Updated: 2026/02/18 15:16:20 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Standard libraries
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Internal libraries
#include <gb.h>
#include <get_next_line.h>
#include <minishell_lb.h>
#include <readline/readline.h>

// Project headers
#include "g_sig.h"
#include "macros/m_colors.h"
#include "modules/ast.h"
#include "modules/builtin.h"
#include "modules/executor.h"
#include "modules/expansion.h"
#include "modules/lexer.h"
#include "modules/parser.h"
#include "modules/signal.h"
#include "modules/utils.h"
#include "structs/s_ast.h"
#include "structs/s_shell.h"

// Local headers
#include "../lexer/token.h"
#include "args.h"
#include "history.h"
#include "read_input.h"
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
