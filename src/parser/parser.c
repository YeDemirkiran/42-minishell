/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/01 02:17:07 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <enums/e_redir_type.h>

#include <structs/s_ast.h>
#include <structs/s_token.h>
#include <structs/s_redirection.h>

#include <modules/ast.h>

static char	**linked_list_to_string_arr(t_list *list)
{
	size_t	length;
	size_t	i;
	char	**args;

	if (list == NULL)
		return (NULL);
	length = ft_lstsize(list);
	args = malloc(sizeof(char *) * (length + 1));
	if (args == NULL)
		return (NULL);
	i = 0;
	while (list)
	{
		args[i++] = list->content;
		list = list->next;
	}
	args[i] = NULL;
	return (args);
}

// Searches the tokens until LENGTH, and retrieves strings in char** format.
// (Ignores non-string)
static char	**tokens_strings_to_args(t_token *tokens, size_t length)
{
	size_t	i;
	t_list	*list;
	t_list	*current;
	char	**args;

	if (tokens == NULL)
		return (NULL);
	list = NULL;
	i = 0;
	while (i < length)
	{
		if ((i == 0 || tokens[i - 1].type != TOKEN_REDIR)
			&& tokens[i].type == TOKEN_STRING)
		{
			current = ft_lstnew(tokens[i].data);
			if (current == NULL)
			{
				ft_lstclear(&list, free);
				return (NULL);
			}
			ft_lstadd_back(&list, current);
		}
		i++;
	}
	args = linked_list_to_string_arr(list);
	ft_lstclear(&list, NULL);
	return (args);
}

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

// Uses tokens from a lexer and converts them into a full AST
// TODO: Remove debug logs
t_ast	*parse(t_token *tokens)
{
	size_t	i;
	size_t	start;
	t_ast	*root;
	t_ast	*current;
	t_ast	*tmp;

	// todo: remove
	printf("(DEBUG: Starting shell step 2: Parser)\n");
	printf("(DEBUG: Step 1: Parse AST)\n");

	if (tokens == NULL)
		return (NULL);
	i = 0;
	start = 0;
	root = NULL;
	current = NULL;
	while (tokens[i].type != TOKEN_EOF)
	{
		if (tokens[i].type == TOKEN_PIPE)
		{
			tmp = ast_new_node(AST_COMMAND, ast_command_data(tokens[start].data,
					tokens_strings_to_args(tokens + start, i - start), NULL),
					NULL, NULL);
			tmp = ast_new_node(AST_PIPE, NULL, tmp, NULL);
			if (root == NULL)
				root = tmp;
			if (current != NULL)
				current->right = tmp;
			current = tmp;
			start = i + 1;
		}
		i++;
	}
	if (i == start)
		return (NULL);
	tmp = ast_new_node(AST_COMMAND, ast_command_data(tokens[start].data,
					tokens_strings_to_args(tokens + start, i - start), NULL),
					NULL, NULL);
	if (root == NULL)
		root = tmp;
	else
		current->right = tmp;
	printf("(DEBUG: Step 1: Printing AST %i)\n", root == NULL);
	ast_traverse_preorder(root, debug_ast_print_node);
	// TODO: Use a helper free_tokens here function that frees tokens
	return (root);
}
