/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/07 18:11:10 by yademirk         ###   ########.fr       */
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
#include "parse_pipeline.h"

/* =============================== FUNCTIONS =============================== */

// // TODO: remove
// static void	debug_ast_print_node(t_ast *ast)
// {
// 	t_list	*list;

// 	printf("\nAST NODE\n");
// 	printf("TYPE: ");
// 	if (ast->type == AST_COMMAND)
// 		printf("command");
// 	else if (ast->type == AST_PIPE)
// 		printf("pipe");
// 	printf("\n");
// 	if (ast->type == AST_COMMAND)
// 	{
// 		printf("COMMAND DATA\n");

// 		list = ast->data->ast_command.arguments;
// 		if (list != NULL)
// 		{
// 			printf("Command: %s\n", (char *)(list->content));
// 			printf("Arguments: ");
// 			while (list != NULL)
// 			{
// 				printf("%s", (char *)(list->content));
// 				if (list->next != NULL)
// 					printf(", ");
// 				list = list->next;
// 			}
// 			printf("\n");
// 		}
// 		else
// 			printf("Arguments: (null)\n");

// 		printf("Redirs: ");
// 		t_list	*lst = ast->data->ast_command.redirs;
// 		if (lst == NULL)
// 		{
// 			printf("(none)\n");
// 			return;
// 		}
// 		t_redirection *redir;
// 		while (lst != NULL)
// 		{
// 			redir = (t_redirection *)(lst->content);

// 			printf("(type: ");
// 			switch (redir->redir_type)
// 			{
// 				case REDIR_APPEND:
// 					printf(">>");
// 					break;
// 				case REDIR_HEREDOC:
// 					printf("<<");
// 					break;
// 				case REDIR_INPUT:
// 					printf("<");
// 					break;
// 				case REDIR_OUTPUT:
// 					printf(">");
// 					break;
// 				default:
// 					break;
// 			}
// 			printf(", fd: %i, path: %s) ", redir->target_fd, redir->path);
// 			lst = lst->next;
// 		}
// 		printf("\n");
// 	}
// }

static char	*get_token_string(t_token *token)
{
	if (token == NULL)
		return (NULL);
	if (token->type == TOKEN_WORD)
		return (token->data.word);
	if (token->type == TOKEN_NEWLINE)
		return ("newline");
	if (token->type == TOKEN_PIPE)
		return ("|");
	if (token->type == TOKEN_IF_AND)
		return ("&&");
	if (token->type == TOKEN_IF_OR)
		return ("||");
	if (token->type == TOKEN_PAREN_LEFT)
		return ("(");
	if (token->type == TOKEN_PAREN_RIGHT)
		return (")");
	if (token->type == TOKEN_REDIR_INPUT)
		return ("<");
	if (token->type == TOKEN_REDIR_OUTPUT)
		return (">");
	if (token->type == TOKEN_REDIR_HEREDOC)
		return ("<<");
	if (token->type == TOKEN_REDIR_APPEND)
		return (">>");
	return (NULL);
}

static void	print_token_error(t_parse_result result)
{
	t_token	*token;
	char	*token_word;

	if (result.type != PARSE_RESULT_TOKEN_ERROR
		&& result.type != PARSE_RESULT_TOKEN_UNCOMPLETE)
		return ;
	if (result.current_token == NULL || result.current_token->content == NULL)
		token = NULL;
	else
		token = (t_token *)(result.current_token->content);
	if (result.type == PARSE_RESULT_TOKEN_ERROR)
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	else if (result.type == PARSE_RESULT_TOKEN_UNCOMPLETE)
		ft_putstr_fd("minishell: uncompleted operator '", 2);
	token_word = get_token_string(token);
	if (token_word == NULL)
		ft_putstr_fd("(null)", 2);
	else
		ft_putstr_fd(token_word, 2);
	ft_putstr_fd("'\n", 2);
}

/**
 * @brief Main entry point for the parser. Calls the required grammar rules.
 */
static t_parse_result	parse_token(t_list *tokens, t_shell shell)
{
	t_parse_result	result;

	if (tokens == NULL || tokens->content == NULL)
		return (parse_runtime_error(NULL));
	result = parse_pipeline(tokens, shell);
	return (result);
}

/**
 * @brief Takes a list of tokens and parses it as a syntax tree.
 * @returns Full AST on success, NULL on failure.
 * @note Prints the error itself in case of failure.
 */
t_ast	*parse(t_list *tokens, t_shell *shell)
{
	t_ast			*root;
	t_parse_result	result;

	//printf("(DEBUG: Starting shell step 2: Parser)\n");
	//printf("(DEBUG: Step 2: Parse AST)\n");
	if (tokens == NULL || tokens->content == NULL)
		return (NULL);
	result = parse_token(tokens, *shell);
	if (result.type != PARSE_RESULT_SUCCESS)
	{
		shell->exit_status = 1;
		if (result.type == PARSE_RESULT_TOKEN_ERROR
			|| result.type == PARSE_RESULT_TOKEN_UNCOMPLETE)
		{
			shell->exit_status = 2;
			print_token_error(result);
		}
		else if (result.type == PARSE_RESULT_RUNTIME_ERROR && errno != 0)
			perror("minishell");
		else
			ft_putendl_fd("minishell: unkown error", 2);
		return (NULL);
	}
	root = result.ast;
	//printf("(DEBUG: Step 2.1: Printing AST)\n");
	//ast_traverse_preorder(root, debug_ast_print_node);
	return (root);
}

/* ================================== END ================================== */
