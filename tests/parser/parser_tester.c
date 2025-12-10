/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:14:20 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/08 18:31:22 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libft/string.h>

#include <structs/s_ast.h>
#include <structs/s_token.h>
#include <structs/s_redirection.h>
#include <types/t_bool.h>

#include <modules/parser.h>
#include <modules/ast.h>

#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_WHITE "\e[0;37m"

typedef struct s_parser_test
{
	char	*name;
	int		major_number;
	int		minor_number;
	t_token	*tokens;
	size_t	token_length;
	t_ast	*expected_ast;
}				t_parser_test;

typedef struct s_test_result
{
	int	total;
	int	passed;
	int	failed;
}	t_test_result;

t_list	*token_list_to_linked_list(t_token *tokens, size_t length)
{
	t_list	*list;
	size_t	i;

	if (tokens == NULL)
		return (NULL);
	i = 0;
	list = NULL;
	while (i < length)
	{
		ft_lstadd_back(&list ,ft_lstnew((void *)tokens + i));
		i++;
	}
	return (list);
}

t_bool	compare_asts(t_ast *expected, t_ast *testee)
{
	// Here, we just check if their nullness is different
	if ((expected == NULL) != (testee == NULL))
		return (0);
	// If both are NULL, return 1
	if (expected == NULL) //
		return (1);

	// Node-level checks
	if (expected->type != testee->type)
		return (0);
	else if ((expected->left != NULL) != (testee->left != NULL))
		return (0);
	else if ((expected->right != NULL) != (testee->right != NULL))
		return (0);

	// Type checks
	if (expected->type == AST_COMMAND)
	{
		char	**expected_args = expected->data->ast_command.arguments;
		char	**testee_args = testee->data->ast_command.arguments;

		// Comamnd path
		if (strcmp(expected->data->ast_command.command,
			testee->data->ast_command.command) != 0)
			return (0);

		// Arguments
		size_t	i = 0;
		while (expected_args[i] != NULL && testee_args[i] != NULL)
		{
			if (strcmp(expected_args[i], testee_args[i]) != 0)
				return (0);
			i++;
		}
		//  Check for early termination
		if ((expected_args[i] != NULL) || (testee_args[i] != NULL))
			return (0);

		// Redirections
		t_list	*exp_redirs = expected->data->ast_command.redirs;
		t_redirection	*exp_redir;
		t_list	*t_redirs = testee->data->ast_command.redirs;
		t_redirection	*t_redir;
		while (exp_redirs != NULL && t_redirs != NULL)
		{
			if ((exp_redirs->content != NULL) != (t_redirs->content != NULL))
				return (0);
			if (exp_redirs->content != NULL)
			{
				exp_redir = (t_redirection *)(exp_redirs->content);
				t_redir = (t_redirection *)(t_redirs->content);
				if (exp_redir->redir_type != t_redir->redir_type)
					return (0);
				else if (strcmp(exp_redir->path, t_redir->path) != 0)
					return (0);
				else if (exp_redir->target_fd != t_redir->target_fd)
					return (0);
			}
			exp_redirs = exp_redirs->next;
			t_redirs = t_redirs->next;
		}
		//  Check for early termination
		if ((exp_redirs != NULL) || (t_redirs != NULL))
			return (0);
	}

	// Tree level checks
	if (expected->left)
	{
		if (compare_asts(expected->left, testee->left) == 0)
			return (0);
	}
	if (expected->right)
	{
		if (compare_asts(expected->right, testee->right) == 0)
			return (0);
	}

	// All checks complete, Structs match
	return (1);
}

void	free_test(t_parser_test test)
{
	free(test.name);
}

t_test_result	conduct_parser_test(t_parser_test test)
{
	t_ast	*retrieved_ast;
	t_bool	result;
	static t_test_result test_result;

	printf("Test %i.%i: %s\n", test.major_number, test.minor_number, test.name);

	printf("Calling parse()...\n");
	printf("=============================================================\n");
	retrieved_ast = parse(token_list_to_linked_list(test.tokens, test.token_length));
	printf("=============================================================\n");

	result = compare_asts(test.expected_ast, retrieved_ast);
	test_result.passed += result == 1;
	test_result.failed += result == 0;
	test_result.total += 1;

	if (test.expected_ast != NULL)
		ast_free_tree(test.expected_ast);
	if (retrieved_ast != NULL)
		ast_free_tree(retrieved_ast);

	return (test_result);
}

void	test_parser(int	test_number)
{
	t_parser_test	tests[] = {
		{
			.name = "ls",
			.major_number = test_number,
			.minor_number = 1,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
			},
			.token_length = 1,
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("ls", ' '), NULL),
				NULL, NULL)
		}
	};

	t_test_result results;

	printf("\nTest %i: Testing parser() function\n", test_number);

	for (size_t i = 0; i < 1; i++)
	{
		results = conduct_parser_test(tests[i]);
	}

	printf("\n==================[RESULTS]==================\n\n");
	printf(COLOR_GREEN "PASS: " COLOR_WHITE "%i/%i\n", results.passed, results.total);
	printf(COLOR_RED "FAIL: " COLOR_WHITE "%i/%i\n", results.failed, results.passed);
	printf("\n[TEST %i DONE]\n", test_number);
}

int	main(void)
{
	printf("\nPARSER TESTER\n");

	test_parser(1);
	return (EXIT_SUCCESS);
}
