/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/01/07 21:40:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <readline/readline.h>

#include <libft/linked_list.h>
#include <libft/string.h>
#include <libft/predicate.h>
#include <get_next_line.h>
#include <minishell_lb.h>
#include <gb.h>

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

#include "../lexer/token.h"
#include "read_input.h"
#include "history.h"
#include "shell_init.h"

/**
 * @brief Prints an optional exit messsage depending on 'interactive'
 * and exits with status.
 */
static void	shell_exit(unsigned char status, t_shell shell)
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

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	t_list	*tokens;
	t_ast	*ast;
	char *prompt;
	int		init_result;

	input = NULL;
	signal(SIGINT, on_sigint);
	init_result = init_shell(&shell, argc, argv, envp);
	if (init_result != 0)
		shell_exit(init_result, shell);
	while (1)
	{
		prompt = rl_header(&(shell.env->envp_list), shell.env->envp_char);
		input = read_input(prompt, "> ", shell.interactive);
		if (input == NULL)
		{
			if (errno != 0)
			{
				shell_error_message(shell, NULL, NULL);
				shell_exit(errno_to_exit_code(), shell);
			}
			break ;
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
		if(ast->type == AST_COMMAND && isbuiltin((char *)(ast->data->ast_command.arguments->content)))
			run_builtin(ast, &(shell.env->envp_char), &(shell.env->envp_list));
		else
			shell.exit_status = executor(ast, &(shell.env), 0, 1);
		ast_free_tree(ast);
		sort_malloc(1, 1);
	}
	shell_exit(shell.exit_status, shell);
}
