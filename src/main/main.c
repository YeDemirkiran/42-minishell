/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 13:18:09 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 15:14:40 by muayna           ###   ########.fr       */
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
#include "main_utils.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Initializes the main function before the first loop.
 *
 * Exits the program early when necessary.
 */

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
	return (0);
}

static int	should_run_builtin(t_ast *ast)
{
	t_list	*args;

	args = ast->data->ast_command.arguments;
	if (ast->type != AST_COMMAND)
		return (0);
	if (args == NULL)
		return (0);
	if (args->content == NULL)
		return (0);
	if (isbuiltin((char *)(args->content)) == 0)
		return (0);
	return (1);
}

static void	run_exec(t_ast *ast, t_shell *shell)
{
	int		std[3];
	t_list	*redir;

	if (ast == NULL)
		return ;
	if (should_run_builtin(ast) != 0)
	{
		if (expansion(ast, *shell) != 1)
			return ;
		std[0] = dup(STDIN_FILENO);
		std[1] = dup(STDOUT_FILENO);
		std[2] = dup(STDERR_FILENO);
		redir = ast->data->ast_command.redirs;
		while (redir != NULL)
			run_redirs(&redir);
		run_builtin(ast, shell, 1);
		dup2(std[0], STDIN_FILENO);
		dup2(std[1], STDOUT_FILENO);
		dup2(std[2], STDERR_FILENO);
		close(std[0]);
		close(std[1]);
		close(std[2]);
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
		shell.root_ast = ast;
		run_exec(ast, &shell);
		ast_free_tree(ast);
		sort_malloc(1, 1);
	}
	shell_exit(shell.exit_status, shell);
}

/* ================================== END ================================== */
