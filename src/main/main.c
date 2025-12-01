/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/01 03:06:46 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>

#include <modules/lexer.h>
#include <modules/ast.h>
#include <modules/parser.h>
#include <modules/executor.h>
#include <modules/builtin.h>
#include <modules/signal.h>

#include <macros/m_colors.h>
#include <structs/s_ast.h>

#define SHELL_NAME "babashell"
#define RL_START "\001"
#define RL_END "\002"

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	char		first_run;
	t_ast		*ast;

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

		// STEP 2: Parser
		// Use the tokens from lexer and parse them to usable commands by the executor.
		ast = parse(tokens);
		(void)ast;

		// STEP 3: Execution
		// Execute the parsed commands here. Provide envp for them.
		// execute(cmd);

		// STEP 4: Restart
		// Prepare the shell for the next iteration.
		// free tokens
		free(tokens);
		ast_free_tree(ast);
	}
	return (EXIT_SUCCESS);
}
