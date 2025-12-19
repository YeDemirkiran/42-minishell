/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/19 13:41:54 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <readline/readline.h>

#include <libft/linked_list.h>
#include <libft/string.h>

#include "macros/m_colors.h"
#include "structs/s_ast.h"

#include "modules/lexer.h"
#include "modules/ast.h"
#include "modules/parser.h"
#include "modules/executor.h"
#include "modules/builtin.h"
#include "modules/signal.h"
#include <get_next_line.h>
#include <minishell_lb.h>
#include <gb.h>

#include "../lexer/token.h"

#define SHELL_NAME "babashell"
#define RL_START "\001"
#define RL_END "\002"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*tmp;
	t_list	*tokens;
	char	first_run;
	t_ast	*ast;
	t_envp_list *envp_list;
	char *prompt;

	envp_list = create_envp_list(envp);
	(void)argc;
	(void)argv;
	(void)envp;
	input = NULL;
	tmp = NULL;
	first_run = 1;
	signal(SIGINT, on_sigint);
	while (input || first_run)
	{
		if (!first_run && input != NULL)
			free(input);
		first_run = 0;
		prompt = rl_header(&envp_list, envp);
		tmp = readline(prompt);
		if (tmp == NULL)
			break ;
		input = ft_strjoin(tmp, "\n");
		free(tmp);
		if (input == NULL)
			break;
		tokens = lexical_analyze(input);
		ast = parse(tokens);
		executor(ast, envp, 0, 1);
		free_tokens(&tokens);
		ast_free_tree(ast);
	}
	return (EXIT_SUCCESS);
}
