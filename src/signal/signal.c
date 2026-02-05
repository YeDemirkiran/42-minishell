/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 23:02:10 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/05 12:45:54 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// For signal.h
#define _POSIX_C_SOURCE 200809L

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

static void	on_sigint_normal(int i)
{
	g_sig = i;
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

	action.sa_handler = on_sigint_normal;
	sigemptyset(&(action.sa_mask));
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_DFL); 
	signal(SIGQUIT, SIG_DFL);
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
