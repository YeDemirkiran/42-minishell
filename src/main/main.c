/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/13 02:02:30 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>

#include <modules/builtin.h>
#include <modules/lexer.h>
#include <modules/signal.h>
#include <modules/executor.h>

#include <macros/m_colors.h>
#include <structs/s_command.h>

#define SHELL_NAME "babashell"
#define RL_START "\001"
#define RL_END "\002"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	char		**tokens;
	char		first_run;
	t_command	cmd;

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
		input = readline(RL_START COLOR_GREEN RL_END SHELL_NAME RL_START COLOR_WHITE RL_END " > ");

		// STEP 1: Lexical Analysis
		// Analyze and tokenize the input.
		tokens = lexical_analyze(input);
		(void)tokens;

		// STEP 2: Parser
		// Use the tokens from lexer and parse them to usable commands by the executor.
		// cmd = parse();

		// STEP 3: Execution
		// Execute the parsed commands here. Provide envp for them.

		cmd.command_path = "/bin/ls";
		cmd.stdin_fd = 0;
		cmd.stdout_fd = 1;
		cmd.stderr_fd = 2;
		executor(&cmd);

		// STEP 4: Restart
		// Prepare the shell for the next iteration.
	}
	return (EXIT_SUCCESS);
}
