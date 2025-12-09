/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/09 18:06:15 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <libft/string.h>
#include "enums/e_redir_type.h"


#include "structs/s_ast.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"

#include "modules/ast.h"

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

		printf("Arguments: ");
		for (size_t i = 0; ast->data->ast_command.arguments[i] != NULL; i++)
		{
			printf("%s", ast->data->ast_command.arguments[i]);
			if (ast->data->ast_command.arguments[i + 1] != NULL)
				printf(", ");
		}
		printf("\n");

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
		printf("\n\n");
	}
}

// Returns the default FD value for a redirection.
static int	redir_default_fd(t_redir_type type)
{
	int	fd;

	fd = -1;
	if (type == REDIR_INPUT || type == REDIR_HEREDOC)
		fd = 0;
	else if (type == REDIR_APPEND || type == REDIR_APPEND)
		fd = 1;
	return (fd);
}

static t_redir_type	get_redir_type(t_token_type type)
{
	int	redir;

	redir = -1;
	if (type == TOKEN_REDIR_INPUT)
		redir = REDIR_INPUT;
	else if (type == TOKEN_REDIR_OUTPUT)
		redir = REDIR_OUTPUT;
	else if (type == TOKEN_REDIR_HEREDOC)
		redir = REDIR_HEREDOC;
	else if (type == TOKEN_REDIR_APPEND)
		redir = REDIR_APPEND;
	return (redir);
}


// Parses a token according to COMMAND grammar.
// TODO: Remove comments.
static t_ast	*parse_command(t_list *token)
{
	t_ast			*command;
	t_token			*token_obj;
	t_list			*redirs;
	t_list			*current_redir;
	t_list			*args;
	t_redirection	*redirection;
	t_redir_type	redir_type;
	int				skip_next;

	if (token == NULL || token->content == NULL)
		return (NULL);
	skip_next = 0;
	current_redir = NULL;
	redirs = NULL;
	args = NULL;
	while (token != NULL)
	{
		token_obj = (t_token *)(token->content);
		if (skip_next == 1)
		{
			skip_next = 0;
			if (token_obj->type == TOKEN_WORD && current_redir != NULL)
				((t_redirection *)(current_redir->content))->path = ft_strdup(token_obj->data.word);
			else
				;
				// throw error
			token = token->next;
			continue ;
		}
		redir_type = get_redir_type(token_obj->type);
		if (redir_type != (t_redir_type)-1)
		{
			redirection = malloc(sizeof(t_redirection));
			if (token_obj->data.number == -1)
				redirection->target_fd = redir_default_fd(redir_type);
			else
				redirection->target_fd = token_obj->data.number;
			redirection->redir_type = redir_type;
			skip_next = 1;
			current_redir = ft_lstnew((void *)redirection);
			ft_lstadd_back(&redirs, current_redir);
		}
		else if (token_obj->type == TOKEN_WORD)
		{
			ft_lstadd_back(&args, ft_lstnew((void *)ft_strdup(token_obj->data.word)));
		}
		else
			break;
		token = token->next;
	}
	if (args == NULL)
		return (NULL);
	command = ast_new_node(AST_COMMAND, ast_command_data(ft_lst_to_string_arr(args), redirs), NULL, NULL);
	ft_lstclear(&args, NULL);
	return (command);
}

// Takes a pointer to a token and generates a tree, and returns it.
// Potentially recursively calls itself.
static t_ast	*parse_token(t_list *token)
{
	t_ast	*ast;
	t_token	*token_obj;

	ast = NULL;
	if (token == NULL || token->content == NULL)
		return (NULL);
	token_obj = (t_token *)(token->content);
	if (token_obj->type == TOKEN_WORD)
		ast = parse_command(token);
	return (ast);
}

// Uses tokens from a lexer and converts them into a full AST
// TODO: Remove debug logs
t_ast	*parse(t_list *tokens)
{
	t_ast	*root;

	// todo: remove printf logs
	printf("(DEBUG: Starting shell step 2: Parser)\n");
	printf("(DEBUG: Step 1: Parse AST)\n");

	if (tokens == NULL)
		return (NULL);
	root = parse_token(tokens);
	printf("(DEBUG: Step 1: Printing AST %i)\n", root == NULL);
	ast_traverse_preorder(root, debug_ast_print_node);
	return (root);
}
