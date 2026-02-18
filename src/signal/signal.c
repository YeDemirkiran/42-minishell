/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:02:10 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:15:27 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

#include "posix_source.h" // for signal.h

// Standxard libraries
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

// Internal libraries
#include <readline/readline.h>

// Project headers
#include "g_sig.h"

volatile sig_atomic_t	g_sig = 0;

/* =============================== FUNCTIONS =============================== */

static void	on_sigint_prompt(int i)
{
	int	interactive;

	rl_done = 1;
	g_sig = i;
	interactive = 0;
	interactive += isatty(STDIN_FILENO);
	interactive += isatty(STDERR_FILENO);
	if (interactive == 2)
		write(STDERR_FILENO, "^C", 2);
	errno = 0;
}

void	install_prompt_signals(void)
{
	struct sigaction	action;

	action.sa_handler = on_sigint_prompt;
	sigemptyset(&(action.sa_mask));
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	install_command_signals(void)
{
	struct sigaction	action;

	action.sa_handler = SIG_IGN;
	sigemptyset(&(action.sa_mask));
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	install_default_signals(void)
{
	struct sigaction	action;

	action.sa_handler = SIG_DFL;
	sigemptyset(&(action.sa_mask));
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	signal(SIGQUIT, SIG_DFL);
}

/* ================================== END ================================== */
