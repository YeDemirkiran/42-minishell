/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/04 16:00:26 by yademirk         ###   ########.fr       */
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

/**
 * @brief Converts a linked list of strings to string array.
 */
// static char	**linked_list_to_string_arr(t_list *list)
// {
// 	size_t	length;
// 	size_t	i;
// 	char	**arr;

// 	if (list == NULL)
// 		return (NULL);
// 	length = ft_lstsize(list);
// 	arr = malloc(sizeof(char *) * (length + 1));
// 	if (arr == NULL)
// 		return (NULL);
// 	i = 0;
// 	while (list)
// 	{
// 		arr[i++] = list->content;
// 		list = list->next;
// 	}
// 	arr[i] = NULL;
// 	return (arr);
// }

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
t_ast	*parse(t_list *tokens)
{
	t_ast	*root;

	// todo: remove printf logs
	printf("(DEBUG: Starting shell step 2: Parser)\n");
	printf("(DEBUG: Step 1: Parse AST)\n");

	if (tokens == NULL)
		return (NULL);
	root = NULL;
	printf("(DEBUG: Step 1: Printing AST)\n");
	ast_traverse_preorder(root, debug_ast_print_node);
	return (root);
}
