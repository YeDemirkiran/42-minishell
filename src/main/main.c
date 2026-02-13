/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid Date        by                   #+#    #+#             */
/*   Updated: 2026/02/13 12:58:31 by yademirk         ###   ########.fr       */
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
 * @brief Initializes the main function before the first loop.
 *
 * Exits the program early when necessary.
 */
static void	init_main(t_shell *shell, int argc, char **argv, char **envp)
{
	int	init_result;

	init_result = init_shell(shell, argc, argv, envp);
	if (init_result != 0)
		shell_exit(init_result, *shell);
	rl_catch_signals = 0;
	rl_event_hook = rl_event;
}

static int	main_prepare_input(t_shell *shell, char **input)
{
	char	*prompt;

	install_prompt_signals();
	prompt = rl_header(&(shell->env->envp_list), shell->env->envp_char);
	*input = read_input(prompt, "> ", shell->interactive);
	if (*input == NULL && (g_sig != SIGINT && errno != EINTR))
	{
		if (errno != 0)
		{
			shell_error_message(*shell, NULL, NULL);
			shell_exit(errno_to_exit_code(), *shell);
		}
		return (1);
	}
	else if (g_sig == SIGINT && errno == EINTR)
	{
		shell->exit_status = 128 + g_sig;
		errno = 0;
		free(*input);
		rl_on_new_line();
		rl_replace_line("", 0);
		return (2);
	}
	return (0);
}

static int	prepare_ast(char *input, t_shell *shell, t_ast **ast_addr)
{
	t_list	*tokens;

	if (shell->interactive == 1)
		add_line_to_history(input);
	tokens = lexical_analyze(input);
	free(input);
	if (tokens == NULL)
	{
		if (errno != 0)
		{
			shell_error_message(*shell, "fatal error", NULL);
			shell->exit_status = 1;
		}
		return (1);
	}
	*ast_addr = parse(tokens, shell);
	free_tokens(&tokens);
	if (*ast_addr == NULL)
		return (1);
	// if (expansion(*ast_addr, *shell) != 1)
	// {
	// 	ast_free_tree(*ast_addr);
	// 	return (1);
	// }
	return (0);
}

static void	run_exec(t_ast *ast, t_shell *shell)
{
	if (ast->type == AST_COMMAND && ast->data->ast_command.arguments != NULL
		&& isbuiltin((char *)(ast->data->ast_command.arguments->content)))
	{
		if (expansion(ast, *shell) != 1)
			return ;
		run_builtin(ast, shell, 1);	
	}
	else
		shell->exit_status = executor(ast, shell, 0, 1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	t_ast	*ast;
	int		result;

	init_main(&shell, argc, argv, envp);
	input = NULL;
	while (1)
	{
		g_sig = 0;
		result = main_prepare_input(&shell, &input);
		if (result == 1)
			break ;
		else if (result == 2)
			continue ;
		result = prepare_ast(input, &shell, &ast);
		if (result == 1)
			continue ;
		run_exec(ast, &shell);
		ast_free_tree(ast);
		sort_malloc(1, 1);
	}
	shell_exit(shell.exit_status, shell);
}

/* ================================== END ================================== */
