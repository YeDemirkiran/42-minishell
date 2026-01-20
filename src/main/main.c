/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/01/20 15:13:47 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

// Internal libraries
#include <readline/readline.h>
#include <libft.h>
#include <get_next_line.h>
#include <minishell_lb.h>
#include <gb.h>

// Project headers
#include "macros/m_colors.h"
#include "structs/s_ast.h"
#include "structs/s_shell.h"
#include "modules/lexer.h"
#include "modules/ast.h"
#include "modules/parser.h"
#include "modules/expansion.h"
#include "modules/executor.h"
#include "modules/builtin.h"
#include "modules/signal.h"
#include "modules/utils.h"
#include "g_sig.h"

// Local headers
#include "../lexer/token.h"
#include "read_input.h"
#include "history.h"
#include "shell_init.h"
#include "args.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Prints an optional exit messsage depending on 'interactive'
 * and exits with status.
 */
void	shell_exit(unsigned char status, t_shell shell)
{
	sort_malloc(1, 1);
	clear_arr(&(shell.env->envp_char));
	if (shell.interactive == 1)
	{
		rl_clear_history();
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
static int	rl_event(void)
{
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	t_list	*tokens;
	t_ast	*ast;
	char *prompt;
	int		init_result;

	if (process_args((const char **)argv, argc) != 1)
		exit(EXIT_SUCCESS);
	input = NULL;
	init_result = init_shell(&shell, argc, argv, envp);
	if (init_result != 0)
		shell_exit(init_result, shell);
	rl_catch_signals = 0;
	rl_event_hook = rl_event;
	while (1)
	{
		g_sig = 0;
		install_prompt_signals();
		prompt = rl_header(&(shell.env->envp_list), shell.env->envp_char);
		input = read_input(prompt, "> ", shell.interactive);
		if (input == NULL && (g_sig != SIGINT && errno != EINTR))
		{
			if (errno != 0)
			{
				shell_error_message(shell, NULL, NULL);
				shell_exit(errno_to_exit_code(), shell);
			}
			break ;
		}
		else if (g_sig == SIGINT && errno == EINTR)
		{
			shell.exit_status = 128 + g_sig;
			g_sig = 0;
			errno = 0;
			free(input);
			rl_on_new_line();
			rl_replace_line("", 0);
			continue ;
		}
		if (shell.interactive == 1)
			add_line_to_history(input);
		tokens = lexical_analyze(input);
		free(input);
		if (tokens == NULL)
		{
			if (errno != 0)
				shell_error_message(shell, "fatal error", NULL);
			continue ;
		}
		ast = parse(tokens, &shell);
		free_tokens(&tokens);
		if (ast == NULL)
			continue ;
		if (expansion(ast, shell) != 1)
		{
			ast_free_tree(ast);
			continue ;
		}
		if(ast->type == AST_COMMAND && ast->data->ast_command.arguments != NULL && isbuiltin((char *)(ast->data->ast_command.arguments->content)))
			run_builtin(ast, &shell);
		else
			shell.exit_status = executor(ast, &shell, 0, 1);
		ast_free_tree(ast);
		sort_malloc(1, 1);
		//VALGRIND_DO_LEAK_CHECK;
	}
	shell_exit(shell.exit_status, shell);
}

/* ================================== END ================================== */
