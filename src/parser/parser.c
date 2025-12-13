/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/13 17:59:57 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdio.h> // TODO: Remove
#include <stdlib.h>
#include <errno.h>

// Internal libraries
#include <libft/string.h>
#include <libft/write.h>

// Project headers
#include "enums/e_redir_type.h"
#include "structs/s_ast.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"

#include "modules/ast.h" // TODO: Remove as it's probably not necessary

// Local headers
#include "parse_command.h"

/* =============================== FUNCTIONS =============================== */

// TODO: remove
static void	debug_ast_print_node(t_ast *ast)
{
	printf("\nAST NODE\n");
	printf("TYPE: ");
	if (ast->type == AST_COMMAND)
		printf("command");
	else if (ast->type == AST_PIPE)
		printf("pipe");
	printf("\n");
	if (ast->type == AST_COMMAND)
	{
		printf("COMMAND DATA\n");
		printf("Command: %s\n", ast->data->ast_command.command);

		if (ast->data->ast_command.arguments != NULL)
		{
			printf("Arguments: ");
			for (size_t i = 0; ast->data->ast_command.arguments[i] != NULL; i++)
			{
				printf("%s", ast->data->ast_command.arguments[i]);
				if (ast->data->ast_command.arguments[i + 1] != NULL)
					printf(", ");
			}
			printf("\n");
		}
		else
			printf("Arguments: (null)\n");

		printf("Redirs: ");
		t_list	*lst = ast->data->ast_command.redirs;
		if (lst == NULL)
		{
			printf("(none)\n");
			return;
		}
		t_redirection *redir;
		while (lst != NULL)
		{
			redir = (t_redirection *)(lst->content);

			printf("(type: ");
			switch (redir->redir_type)
			{
				case REDIR_APPEND:
					printf(">>");
					break;
				case REDIR_HEREDOC:
					printf("<<");
					break;
				case REDIR_INPUT:
					printf("<");
					break;
				case REDIR_OUTPUT:
					printf(">");
					break;
				default:
					break;
			}
			printf(", fd: %i, path: %s) ", redir->target_fd, redir->path);
			lst = lst->next;
		}
		printf("\n");
	}
}

/**
 * @brief Main entry point for the parser. Calls the required grammar rules.
 */
static t_parse_result	parse_token(t_list *tokens)
{
	t_parse_result	result;

	if (tokens == NULL || tokens->content == NULL)
		return (parse_runtime_error(NULL));
	result = parse_command(tokens);
	return (result);
}

static void	print_token_error(t_parse_result result)
{
	t_token	*token;
	char	*token_word;

	if (result.type != PARSE_RESULT_TOKEN_ERROR)
		return ;
	if (result.current_token == NULL || result.current_token->content == NULL)
		token = NULL;
	else
		token = (t_token *)(result.current_token->content);
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	if (token->type == TOKEN_WORD)
		token_word = token->data.word;
	else if (token->type == TOKEN_NEWLINE)
		token_word = "newline";
	else if (token->type == TOKEN_PIPE)
		token_word = "|";
	else if (token->type == TOKEN_IF_AND)
		token_word = "&&";
	else if (token->type == TOKEN_IF_OR)
		token_word = "||";
	else if (token->type == TOKEN_PAREN_LEFT)
		token_word = "(";
	else if (token->type == TOKEN_PAREN_RIGHT)
		token_word = ")";
	else if (token->type == TOKEN_REDIR_INPUT)
		token_word = "<";
	else if (token->type == TOKEN_REDIR_OUTPUT)
		token_word = ">";
	else if (token->type == TOKEN_REDIR_HEREDOC)
		token_word = "<<";
	else if (token->type == TOKEN_REDIR_APPEND)
		token_word = ">>";
	else
		token_word = NULL;
	if (token_word == NULL)
		ft_putstr_fd("(null)", 2);
	else
		ft_putstr_fd(token_word, 2);
	ft_putstr_fd("'\n", 2);
}

/**
 * @brief Takes a list of tokens and parses it as a syntax tree.
 * @returns Full AST on success, NULL on failure.
 * @note Prints the error itself in case of failure.
 */
t_ast	*parse(t_list *tokens)
{
	t_ast			*root;
	t_parse_result	result;

	// todo: remove printf logs
	printf("(DEBUG: Starting shell step 2: Parser)\n");
	printf("(DEBUG: Step 1: Parse AST)\n");

	if (tokens == NULL)
		return (NULL);
	result = parse_token(tokens);
	if (result.type != PARSE_RESULT_SUCCESS)
	{
		// Time to give an error
		if (result.type == PARSE_RESULT_TOKEN_ERROR)
			print_token_error(result);
		else if (result.type == PARSE_RESULT_RUNTIME_ERROR && errno != 0)
			perror("minishell");
		else
			ft_putendl_fd("minishell: unkown error", 2);
		return (NULL);
	}
	root = result.ast;
	printf("(DEBUG: Step 1: Printing AST %i)\n", root == NULL);
	ast_traverse_preorder(root, debug_ast_print_node);
	return (root);
}

/* ================================== END ================================== */
