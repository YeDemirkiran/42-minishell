/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/10 14:40:04 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>

#include <modules/builtin.h>
#include <modules/lexer.h>
#include <modules/signal.h>

#include <macros/m_colors.h>

#define SHELL_NAME "babashell"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	first_run;

	(void)argc;
	(void)argv;
	(void)envp;
	input = NULL;
	first_run = 1;
	signal(SIGINT, on_sigint);
	while (input || first_run)
	{
		if (!first_run)
			free(input);
		first_run = 0;
		input = readline(COLOR_GREEN SHELL_NAME COLOR_WHITE " > ");
		
		// STEP 1: Lexical Analysis
		lexical_analyze(input);
		
		// STEP 2: Parser

		// STEP 3: Execution

		// STEP 4: Restart
	}
	return (EXIT_SUCCESS);
}
