/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tester.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:14:20 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/25 17:02:42 by yademirk         ###   ########.fr       */
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
#include <enums/e_redir_type.h>

#include <modules/expansion.h>
#include <modules/ast.h>

#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_WHITE "\e[0;37m"

typedef struct s_expansion_test
{
	int		number;
	t_ast	*testee;
	t_ast	*expected;
}			t_expansion_test;

typedef struct s_test_result
{
	int	total;
	int	passed;
	int	failed;
}	t_test_result;

t_list	*char_arr_to_list(char **arr)
{
	size_t	i;
	t_list	*root;

	if (arr == NULL)
		return (NULL);
	i = 0;
	root = NULL;
	while (arr[i] != NULL)
	{
		ft_lstadd_back(&root, ft_lstnew((void *)arr[i]));
		i++;
	}
	free(arr);
	return (root);
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
		t_list	*expected_args = expected->data->ast_command.arguments;
		t_list	*testee_args = testee->data->ast_command.arguments;


		// NULL check
		if ((expected_args == NULL) != (testee_args == NULL))
			return (0);

		// Arguments
		if (expected_args != NULL)
		{
			while (expected_args != NULL && testee_args != NULL)
			{
				if (expected_args->content == NULL || testee_args->content == NULL)
				{
					if ((expected_args->content == NULL) != (testee_args->content == NULL))
						return (0);
				}
				else if (strcmp((char *)(expected_args->content), (char *)(testee_args->content)) != 0)
					return (0);
				expected_args = expected_args->next;
				testee_args = testee_args->next;
			}
			//  Check for early termination
			if ((expected_args != NULL) || (testee_args != NULL))
				return (0);
		}

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

t_test_result	conduct_expansion_test(t_expansion_test test)
{
	t_bool	result;
	static t_test_result test_result;

	printf("Test %i\n", test.number);

	printf("Calling expansion()...\n");
	printf("=============================================================\n");
	expansion(test.testee, NULL);
	printf("=============================================================\n\n");

	result = compare_asts(test.expected, test.testee);
	test_result.passed += result == 1;
	test_result.failed += result == 0;
	test_result.total += 1;

	printf("[EXPECTED]  NULL: %s\n", test.expected == NULL ? "Yes" : "No");
	printf("[RETRIEVED] NULL: %s\n", test.testee == NULL ? "Yes" : "No");
	printf("%s[%s]" COLOR_WHITE"\n", result ? COLOR_GREEN : COLOR_RED, result ? "OK" : "KO");

	if (test.testee != NULL)
		ast_free_tree(test.testee);
	if (test.expected != NULL)
		ast_free_tree(test.expected);
	printf("\n");
	return (test_result);
}

int	test_number = 0;

#define TEST_COMMAND(str) ast_new_node(AST_COMMAND, ast_command_data(char_arr_to_list(ft_split(str, ' ')), NULL), NULL, NULL)
#define TEST_COMMAND_NULL ast_new_node(AST_COMMAND, ast_command_data(NULL, NULL), NULL, NULL)
#define TEST_COMMAND_EMPTY ast_new_node(AST_COMMAND, ast_command_data(ft_lstnew(NULL), NULL), NULL, NULL)
#define TEST_PIPE(left, right) ast_new_node(AST_PIPE, NULL, left, right)
#define TEST_EXPANSION(testee_str, expected_str) { .number = (++test_number), .testee = TEST_COMMAND(testee_str), .expected = TEST_COMMAND(expected_str)}
#define TEST_EXPANSION_NULL(testee_str) { .number = (++test_number), .testee = TEST_COMMAND(testee_str), .expected = TEST_COMMAND_NULL}
#define TEST_EXPANSION_EMPTY(testee_str) { .number = (++test_number), .testee = TEST_COMMAND(testee_str), .expected = TEST_COMMAND_EMPTY}

static void	test_expansion(void)
{
	t_expansion_test	tests[] = {
		// TEST 1: Basic tests
		TEST_EXPANSION("$USER", "yademirk"),
		TEST_EXPANSION("echo $USER", "echo yademirk"),
		TEST_EXPANSION("echo $USER two", "echo yademirk two"),
		TEST_EXPANSION("word$USER", "wordyademirk"),
		TEST_EXPANSION("$USER$USER", "yademirkyademirk"),
		TEST_EXPANSION("$USER$USER$USER", "yademirkyademirkyademirk"),
		TEST_EXPANSION("$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER", "yademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirk"),
		TEST_EXPANSION("word1-$USER-word2-$USER-word3-$USER", "word1-yademirk-word2-yademirk-word3-yademirk"),

		// TEST 2: Mixed valid variables
		TEST_EXPANSION("$USER $HOME", "yademirk /home/yademirk"),
		TEST_EXPANSION("$USER$HOME", "yademirk/home/yademirk"),
		TEST_EXPANSION("$USER$HOME$USER", "yademirk/home/yademirkyademirk"),
		TEST_EXPANSION("$USER$USER$HOME$HOME", "yademirkyademirk/home/yademirk/home/yademirk"),
		TEST_EXPANSION("$HOME$PWD$DISPLAY$PATH$USERNAME$DESKTOP_SESSION$SHELL", "/home/yademirk" "/home/yademirk/Desktop/42-projects/minishell/tests/expansion" ":0" "/home/yademirk/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin" "yademirk" "ubuntu" "/bin/bash"),

		// TEST 3: High count of variables
		// 100 here
		TEST_EXPANSION("$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER", "yademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirk"),

		// 1000 here
		TEST_EXPANSION("$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER$USER", "yademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirkyademirk"),

		// TEST 4: Non-existing variables
		TEST_EXPANSION_EMPTY("$no"),
		TEST_EXPANSION("hello$no.world", "hello.world"),
		TEST_EXPANSION("hello$a.world", "hello.world"),
		TEST_EXPANSION("hello$.world", "hello$.world"),
		TEST_EXPANSION("hello$asdasasd$asfagagagf$afafaafafaafaf$1e112$asfafsasf_asfasfasf.world", "hello.world"),
		TEST_EXPANSION("hello$$$$$$$$$$.world", "hello$$$$$$$$$$.world"),

		// TEST 5: Invalid variables
		TEST_EXPANSION("$", "$"),
		TEST_EXPANSION("$$$$$$$$$$", "$$$$$$$$$$"),
		TEST_EXPANSION("$.", "$."),
		TEST_EXPANSION("$...........", "$..........."),
		TEST_EXPANSION("$ ", "$ "),

		// TEST 6: Invalid variables with words
		TEST_EXPANSION("echo $ test", "echo $ test"),
		TEST_EXPANSION("echo $ test $ echo test $ $ $ $", "echo $ test $ echo test $ $ $ $"),
		TEST_EXPANSION("echo $@ test $$$$$$ echo test $@@@@ $... $. $", "echo $@ test $$$$$$ echo test $@@@@ $... $. $"),

		// TEST 7: Single quotes
		TEST_EXPANSION("'$USER'", "'$USER'"),
		TEST_EXPANSION("'$USER$USER'", "'$USER$USER'"),
		TEST_EXPANSION("'$USER''$USER'", "'$USER''$USER'"),
		TEST_EXPANSION("'$USER'$USER'$USER'", "'$USER'yademirk'$USER'"),

		// TEST 8: Double quotes
		TEST_EXPANSION("\"$USER\"", "\"yademirk\""),
		TEST_EXPANSION("\"$USER$USER\"", "\"yademirkyademirk\""),
		TEST_EXPANSION("\"$USER\"\"$USER\"", "\"yademirk\"\"yademirk\""),
		TEST_EXPANSION("\"$USER\"$USER\"$USER\"", "\"yademirk\"yademirk\"yademirk\""),

		// TEST 9: Single quotes within double quotes
		TEST_EXPANSION("\"'$USER'\"", "\"'yademirk'\""),
		TEST_EXPANSION("\"'$USER$USER'\"", "\"'yademirkyademirk'\""),
		TEST_EXPANSION("\"'$USER'\"\"'$USER'\"", "\"'yademirk'\"\"'yademirk'\""),

		// TEST 10: Double quotes within single quotes
		TEST_EXPANSION("'\"$USER\"'", "'\"$USER\"'"),
		TEST_EXPANSION("'\"$USER$USER\"'", "'\"$USER$USER\"'"),
		TEST_EXPANSION("'\"$USER\"''\"$USER\"'", "'\"$USER\"''\"$USER\"'"),

		// TEST 11: Deep nested
		TEST_EXPANSION("'\"\"\"\"\"\"\"\"\"\"$USER\"\"\"\"\"\"\"\"\"\"'", "'\"\"\"\"\"\"\"\"\"\"$USER\"\"\"\"\"\"\"\"\"\"'"),
		TEST_EXPANSION("\"''''''''''$USER''''''''''\"", "\"''''''''''yademirk''''''''''\""),
	};
	t_test_result results;

	printf("\nTesting expansion() function\n\n");

	for (size_t i = 0; i < sizeof(tests) / sizeof(t_expansion_test); i++)
	{
		results = conduct_expansion_test(tests[i]);
	}

	printf("\n==================[RESULTS]==================\n\n");
	printf(COLOR_GREEN "PASS: " COLOR_WHITE "%i/%i\n", results.passed, results.total);
	printf(COLOR_RED "FAIL: " COLOR_WHITE "%i/%i\n", results.failed, results.total);
	printf("\n[TEST DONE]\n");
}

int	main(void)
{
	printf("\nEXPANSION TESTER\n");

	test_expansion();

	return (EXIT_SUCCESS);
}
