/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:56:18 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/02 15:32:56 by yademirk         ###   ########.fr       */
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

static void	set_quote(char c, char *quote_addr)
{
	if (c == '\'' && *quote_addr != '"')
	{
		if (*quote_addr == '\'')
			*quote_addr = 0;
		else
			*quote_addr = '\'';
	}
	else if (c == '"' && *quote_addr != '\'')
	{
		if (*quote_addr == '"')
			*quote_addr = 0;
		else
			*quote_addr = '"';
	}
}

/**
 * @brief Checks if an input has unclosed quotes, and returns the type of
 * quote.
 * @return 0 (nul) character if input is complete, and the uncomplete quote
 * character if it's not.
 */
static char	is_input_incomplete(char *input)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i] != '\0')
	{
		set_quote(input[i], &quote);
		i++;
	}
	return (quote);
}

static void	print_uncompleted_eof_error(char quote)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching ", 2);
	if (quote == '"')
		ft_putstr_fd("'\"'", 2);
	else
		ft_putstr_fd("(')", 2);
	ft_putstr_fd("\nminishell: syntax error: unexpected end of file\n", 2);
}

/**
 * @brief Uses GNU Readline, prepares a multi-line input by the user
 * and returns it.
 */
static char	*read_input(char *primary_prompt, char *secondary_prompt)
{
	char	*input;
	char	*tmp;
	char	*tmp_2;
	char	*prompt;
	char	quote;

	errno = 0;
	input = NULL;
	quote = 0;
	while (1)
	{
		if (input == NULL)
			prompt = primary_prompt;
		else
			prompt = secondary_prompt;
		tmp = readline(prompt);
		if (tmp == NULL)
		{
			if (input != NULL)
			{
				free(input);
				input = NULL;
				print_uncompleted_eof_error(quote);
				continue ;
			}
			else
				break ;
		}
		tmp_2 = ft_strjoin(tmp, "\n");
		free(tmp);
		if (tmp_2 == NULL)
		{
			if (input != NULL)
				free(input);
			return (NULL);
		}
		tmp = tmp_2;
		if (input == NULL)
			input = tmp;
		else
		{
			tmp_2 = ft_strjoin(input, tmp);
			free(tmp);
			free(input);
			if (tmp_2 == NULL)
				return (NULL);
			input = tmp_2;
		}
		quote = is_input_incomplete(input);
		if (quote == 0)
			break ;
	}
	return (input);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_list	*tokens;
	char	first_run;
	t_ast	*ast;
	char *prompt;
	t_god_struct_envp *envp_struct;

	envp_struct = ft_malloc(sizeof(t_god_struct_envp), 0);
	envp_struct->envp_list = create_envp_list(envp);
	envp_struct->envp_char = envp_list_to_char_arr(envp_struct->envp_list);
	(void)argc;
	(void)argv;
	(void)envp;
	input = NULL;
	first_run = 1;
	signal(SIGINT, on_sigint);
	while (input || first_run)
	{
		if (!first_run && input != NULL)
			free(input);
		first_run = 0;
		prompt = rl_header(&envp_struct->envp_list, envp);
		input = read_input(prompt, "> ");
		if (input == NULL)
		{
			if (errno != 0)
			{
				perror("minishell");
				continue ;
			}
			break ;
		}
		tokens = lexical_analyze(input);
		ast = parse(tokens);
		if (expansion(ast, NULL) != 1)
		{
			free_tokens(&tokens);
			ast_free_tree(ast);
			continue ;
		}
		if(ast->type == AST_COMMAND && isbuiltin((char *)(ast->data->ast_command.arguments->content)))
			run_builtin(ast, &envp_struct->envp_char, &envp_struct->envp_list);
		else
			executor(ast, &envp_struct, 0, 1);
		free_tokens(&tokens);
		ast_free_tree(ast);
		sort_malloc(1, 1);
	}
	sort_malloc(1, 1);
	clear_arr(&envp_struct->envp_char);
	ft_putstr_fd("exit\n", 2);
	return (EXIT_SUCCESS);
}
