/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 15:06:09 by yademirk         ###   ########.fr       */
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
#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"

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

static void manuel_test(t_ast **ast)
{
	(void)ast;
	   	//(*ast) = ast_new_node(AST_COMMAND, ast_command_data(ft_split("ls", ' '), NULL), NULL , NULL);
		//ast = ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("ls", ' '), NULL), NULL, NULL), ast_new_node(AST_COMMAND, ast_command_data(ft_split("wc -l", ' '), NULL), NULL, NULL));
		//(*ast) = ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("cat Makefile", ' '), NULL), NULL, NULL), ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("grep a", ' '), NULL), NULL, NULL), ast_new_node(AST_COMMAND, ast_command_data(ft_split("grep o", ' '), NULL), NULL, NULL)));
		//(*ast) = ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("cat /etc/passwd", ' '), NULL), NULL, NULL), ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("grep root", ' '), NULL), NULL, NULL), ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("cut -d: -f1", ' '), NULL), NULL, NULL), ast_new_node(AST_COMMAND, ast_command_data(ft_split("wc -l", ' '), NULL), NULL, NULL))));
		// (*ast) = ast_new_node(AST_COMMAND, ast_command_data(ft_split("cat", ' '), ft_lstnew(&((t_redirection){.path = "Makefile",.redir_type = REDIR_INPUT, .target_fd = 0}))), NULL, NULL);
		// (*ast) = ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("cat", ' '), ft_lstnew(&((t_redirection){.path = "Makefile",.redir_type = REDIR_INPUT, .target_fd = 0}))), NULL, NULL), ast_new_node(AST_COMMAND, ast_command_data(ft_split("wc -l", ' '), NULL), NULL, NULL));
}

int	main(int argc, char **argv, char **envp)
{
	int fd_in = 0;
	int fd_out = 1;
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
		ast = ast_new_node(AST_PIPE, NULL, ast_new_node(AST_COMMAND, ast_command_data(ft_split("cat", ' '), ft_lstnew(&((t_redirection){.path = "Makefile",.redir_type = REDIR_INPUT, .target_fd = 0}))), NULL, NULL), ast_new_node(AST_COMMAND, ast_command_data(ft_split("wc -l", ' '), ft_lstnew(&((t_redirection){.path = "out",.redir_type = REDIR_OUTPUT, .target_fd = 1}))), NULL, NULL));
		(void)manuel_test;
		//manuel_test(&ast);
		executor(ast, envp, fd_in, fd_out);
		if (input == NULL)
			break;
	}
	return (EXIT_SUCCESS);
}
