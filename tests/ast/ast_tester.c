/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tester.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:34:34 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/29 04:38:02 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <modules/ast.h>
#include <structs/s_redirection.h>
#include <enums/e_redir_type.h>
#include <libft.h>

void	ast_print_node(t_ast *ast)
{
	printf("AST NODE\n");
	printf("TYPE: ");
	if (ast->type == AST_COMMAND)
		printf("command");
	else if (ast->type == AST_PIPE)
		printf("pipe");
	printf("\n");
	if (ast->type == AST_COMMAND)
	{
		printf("COMMAND DATA\n");
		printf("Command: %s\n", ast->data.ast_command.command);

		printf("Arguments: ");
		for (size_t i = 0; ast->data.ast_command.arguments[i] != NULL; i++)
		{
			printf("%s", ast->data.ast_command.arguments[i]);
			if (ast->data.ast_command.arguments[i + 1] != NULL)
				printf(", ");
		}
		printf("\n");

		printf("Redirs: ");
		t_list	*lst = ast->data.ast_command.redirs;
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
		}
		printf("\n");
	}
}


void	test_ast(void)
{
	t_ast		*tree;
	t_ast_data	data;
	char		*command;
	char		**argv;
	t_list		*redirs;

	printf("Test 1: ast_command_data()\n");
	command = ft_strdup("ls");
	argv = malloc(sizeof(char *) * 2);
	argv[0] = command;
	argv[1] = NULL;
	redirs = NULL;
	data = ast_command_data(command, argv, redirs);

	printf("Test 2: ast_new_node()\n");
	tree = ast_new_node(AST_COMMAND, data, NULL, NULL);
	if (tree == NULL)
	{
		printf("FAIL: ast_new_node returned NULL\n");
		return ;
	}

	printf("Test 3: ast_traverse_preorder() with ast_print_node()\n");
	ast_traverse_preorder(tree, ast_print_node);

	printf("Test 4: ast_traverse_postorder() with ast_print_node()\n");
	ast_traverse_postorder(tree, ast_print_node);

	printf("Test 5: ast_free_tree\n");
	ast_free_tree(tree);
}

int	main(void)
{
	printf("\nAST TESTER\n");

	test_ast();

	return (EXIT_SUCCESS);
}
