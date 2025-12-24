/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/24 12:08:25 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
#include "modules/expansion.h"
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
	char **envp_char_list;
	char *prompt;

	envp_list = create_envp_list(envp);
	envp_char_list = envp_list_to_char_arr(envp_list);
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
		//prompt = rl_header(&envp_list, envp);
		prompt = RL_START COLOR_GREEN "babashell" COLOR_WHITE RL_END " > ";
		tmp = readline(prompt);
		if (tmp == NULL)
			break ;
		input = ft_strjoin(tmp, "\n");
		free(tmp);
		if (input == NULL)
			break;
		tokens = lexical_analyze(input);
		ast = parse(tokens);
		expansion(ast, NULL);
		if(ast->type == AST_COMMAND && isbuiltin((char *)(ast->data->ast_command.arguments->content)))
			run_builtin(ast, &envp_char_list, &envp_list);
		else
			executor(ast, envp_char_list, 0, 1);
		free_tokens(&tokens);
		ast_free_tree(ast);
		//free(prompt);
	}
	free(prompt);
	return (EXIT_SUCCESS);
}
