/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/10 17:12:48 by muayna           ###   ########.fr       */
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

#include "../lexer/token.h"

#define SHELL_NAME "babashell"
#define RL_START "\001"
#define RL_END "\002"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	*tmp;
	//t_list	*tokens;
	char	first_run;
	t_ast	*ast;

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
		tmp = readline(RL_START COLOR_GREEN RL_END SHELL_NAME RL_START COLOR_WHITE RL_END " > ");
		if (tmp == NULL)
			break ;
		input = ft_strjoin(tmp, "\n");
		free(tmp);
		ast = ast_new_node(AST_COMMAND, ast_command_data(ft_split(input, ' '), NULL), NULL , NULL);
		//printf("DEBUG COMMAND : %s\n", ast->data->ast_command.command);
		executor(ast, envp);
		if (input == NULL)
			break;
	}
	return (EXIT_SUCCESS);
}
