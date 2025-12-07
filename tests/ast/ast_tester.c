/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tester.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 00:34:34 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/29 16:42:54 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include <modules/ast.h>
#include <structs/s_redirection.h>
#include <enums/e_redir_type.h>
#include <libft.h>

void	ast_print_node(t_ast *ast)
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


void	test_ast(void)
{
	t_ast		*tree;
	t_ast_data	*data[3];
	char		*command;
	char		**argv;
	t_list		*redirs;

	printf("Test 1: ast_command_data()\n");
	command = ft_strdup("cat");
	argv = malloc(sizeof(char *) * 2);
	argv[0] = command;
	argv[1] = NULL;
	redirs = ft_lstnew((void *)(&(t_redirection){.path = ft_strdup("./Makefile"), .redir_type = REDIR_INPUT, .target_fd = 0}));
	data[0] = ast_command_data(command, argv, redirs);

	command = ft_strdup("grep");
	argv = malloc(sizeof(char *) * 3);
	argv[0] = command;
	argv[1] = ft_strdup("c");
	argv[2] = NULL;
	redirs = NULL;
	data[1] = ast_command_data(command, argv, redirs);

	command = ft_strdup("wc");
	argv = malloc(sizeof(char *) * 3);
	argv[0] = command;
	argv[1] = ft_strdup("-l");
	argv[2] = NULL;
	redirs = NULL;
	data[2] = ast_command_data(command, argv, redirs);

	printf("Test 2: ast_new_node()\n");
	tree = ast_new_node(AST_PIPE, NULL,
		ast_new_node(AST_COMMAND, data[0], NULL, NULL),
		ast_new_node(AST_PIPE, NULL, 
			ast_new_node(AST_COMMAND, data[1], NULL, NULL),
			ast_new_node(AST_COMMAND, data[2], NULL, NULL)));

	if (tree == NULL)
	{
		printf("FAIL: ast_new_node returned NULL\n");
		return ;
	}

	printf("Test 3: ast_traverse_preorder() with ast_print_node()\n");
	ast_traverse_preorder(tree, ast_print_node);

	printf("\nTest 4: ast_traverse_postorder() with ast_print_node()\n");
	ast_traverse_postorder(tree, ast_print_node);

	printf("\nTest 5: ast_free_tree\n");
	ast_free_tree(tree);
}

int	main(void)
{
	printf("\nAST TESTER\n");

	test_ast();

	return (EXIT_SUCCESS);
}
