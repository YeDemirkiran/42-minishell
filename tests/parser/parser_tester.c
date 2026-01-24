/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:14:20 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/24 16:00:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libft/string.h>

#include <structs/s_ast.h>
#include <structs/s_token.h>
#include <structs/s_redirection.h>
#include <types/t_bool.h>
#include <enums/e_redir_type.h>

#include <modules/parser.h>
#include <modules/ast.h>

#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_WHITE "\e[0;37m"

t_list	*char_arr_to_list(char **arr)
{
	size_t	i;
	t_list	*root;
	t_list	*node;

	i = 0;
	root = NULL;
	node = NULL;
	while (arr[i] != NULL)
	{
		if (node == NULL)
			node = ft_lstnew((void *)arr[i]);
		else
		{
			node->next = ft_lstnew((void *)arr[i]);
			node = node->next;
		}
		if (root == NULL)
			root = node;
		i++;
	}
	free(arr);
	return (root);
}

#define TEST_COMMAND(str) ast_new_node(AST_COMMAND, ast_command_data(char_arr_to_list(ft_split(str, ' ')), NULL), NULL, NULL)
#define TEST_COMMAND_REDIR(str, redir_type, redir_fd, redir_path) ast_new_node(AST_COMMAND, ast_command_data(char_arr_to_list(ft_split(str, ' ')), ft_lstnew(alloc_redir(redir_type, redir_fd, ft_strdup(redir_path)))), NULL, NULL)
#define TEST_REDIR(redir_type, redir_fd, redir_path) ast_new_node(AST_COMMAND, ast_command_data(NULL, ft_lstnew(alloc_redir(redir_type, redir_fd, ft_strdup(redir_path)))), NULL, NULL)
#define TEST_PIPE(left, right) ast_new_node(AST_PIPE, NULL, left, right)

#ifdef MINISHELL_BONUS
# define TEST_AND(left, right) ast_new_node(AST_IF, NULL, left, right)
# define TEST_OR(left, right) ast_new_node(AST_OR, NULL, left, right)
# define TEST_SUBSHELL(left) ast_new_node(AST_SUBSHELL, NULL, left, NULL)
#endif

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
		ft_lstadd_back(&list, ft_lstnew((void *)(tokens + i)));
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
				if (strcmp((char *)(expected_args->content), (char *)(testee_args->content)) != 0)
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
				else if (t_redir->redir_type == REDIR_HEREDOC)
				{
					unlink(t_redir->path);
				}
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
	t_list	*list;
	static t_shell	shell;

	printf("Test %i.%i: %s\n", test.major_number, test.minor_number, test.name);

	printf("Calling parse()...\n");
	printf("=============================================================\n");
	list = token_list_to_linked_list(test.tokens, test.token_length);
	retrieved_ast = parse(list, &shell);
	printf("=============================================================\n\n");

	result = compare_asts(test.expected_ast, retrieved_ast);
	test_result.passed += result == 1;
	test_result.failed += result == 0;
	test_result.total += 1;

	printf("[EXPECTED]  NULL: %s\n", test.expected_ast == NULL ? "Yes" : "No");
	printf("[RETRIEVED] NULL: %s\n", retrieved_ast == NULL ? "Yes" : "No");
	printf("%s[%s]" COLOR_WHITE"\n", result ? COLOR_GREEN : COLOR_RED, result ? "OK" : "KO");

	if (test.expected_ast != NULL)
		ast_free_tree(test.expected_ast);
	if (retrieved_ast != NULL)
		ast_free_tree(retrieved_ast);
	ft_lstclear(&list, NULL);

	printf("\n");
	return (test_result);
}

static void *alloc_redir(t_redir_type type, int fd, char *path)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	redir->path = path;
	redir->redir_type = type;
	redir->target_fd = fd;
	return ((void *)redir);
}

// static t_list	*two_redir_list(t_redirection first, t_redirection second)
// {
// 	t_list	*root;

// 	root = ft_lstnew(alloc_redir(first.redir_type, first.target_fd, first.path));
// 	root->next = ft_lstnew(alloc_redir(second.redir_type, second.target_fd, second.path));
// 	return (root);
// }

// static t_list	*four_redir_list(t_redirection first, t_redirection second, t_redirection third, t_redirection fourth)
// {
// 	t_list	*root;

// 	root = two_redir_list(first, second);
// 	root->next->next = two_redir_list(third, fourth);
// 	return (root);
// }

void	test_parser(int	test_number)
{
	t_parser_test	tests[] = {

		// CASE 1: SIMPLE COMMANDS (NO REDIRS)
		{
			.name = "ls",
			.major_number = test_number,
			.minor_number = 1,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
			},
			.token_length = 1,
			.expected_ast = TEST_COMMAND("ls")
		},
		{
			.name = "ls -l -a",
			.major_number = test_number,
			.minor_number = 2,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_WORD, .data.word = "-l" },
				{ .type = TOKEN_WORD, .data.word = "-a" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND("ls -l -a")
		},
		{
			.name = "Only Newline",
			.major_number = test_number,
			.minor_number = 3,
			.tokens = (t_token[]) {
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "Long Command",
			.major_number = test_number,
			.minor_number = 4,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
				{ .type = TOKEN_WORD, .data.word = "README.md" },
				{ .type = TOKEN_WORD, .data.word = "this_file_doesnt_exist_lol" },
				{ .type = TOKEN_WORD, .data.word = "BROTHER_HOW" },
				{ .type = TOKEN_WORD, .data.word = "Im_running_out_of_shit_to_test" },
			},
			.token_length = 6,
			.expected_ast = TEST_COMMAND("cat Makefile README.md this_file_doesnt_exist_lol BROTHER_HOW Im_running_out_of_shit_to_test")
		},

		// CASE 2: SIMPLE COMMANDS WITH REDIRECTIONS
		{
			.name = "cat < Makefile",
			.major_number = test_number,
			.minor_number = 5,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_INPUT, 0, "Makefile")
		},
		{
			.name = "cat << EOF",
			.major_number = test_number,
			.minor_number = 6,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "EOF" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_HEREDOC, 0, "EOF")
		},
		{
			.name = "cat > File",
			.major_number = test_number,
			.minor_number = 7,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_OUTPUT, 1, "File")
		},
		{
			.name = "cat >> File",
			.major_number = test_number,
			.minor_number = 8,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_APPEND, 1, "File")
		},

		// CASE 3: REDIRECTIONS WITH FDs
		{
			.name = "cat 999999999< Makefile",
			.major_number = test_number,
			.minor_number = 9,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = 999999999 },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_INPUT, 999999999, "Makefile")
		},
		{
			.name = "cat 999999999<< EOF",
			.major_number = test_number,
			.minor_number = 10,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = 999999999 },
				{ .type = TOKEN_WORD, .data.word = "EOF" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_HEREDOC, 999999999, "EOF")
		},
		{
			.name = "cat 999999999> File",
			.major_number = test_number,
			.minor_number = 11,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = 999999999 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_OUTPUT, 999999999, "File")
		},
		{
			.name = "cat 999999999>> File",
			.major_number = test_number,
			.minor_number = 12,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_APPEND, .data.number = 999999999 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 3,
			.expected_ast = TEST_COMMAND_REDIR("cat", REDIR_APPEND, 999999999, "File")
		},

		// // CASE 4: MULTIPLE REDIRECTIONS
		// {
		// 	.name = "cat < Makefile < Makefile2",
		// 	.major_number = test_number,
		// 	.minor_number = 13,
		// 	.tokens = (t_token[]) {
		// 		{ .type = TOKEN_WORD, .data.word = "cat" },
		// 		{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
		// 		{ .type = TOKEN_WORD, .data.word = "Makefile" },
		// 		{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
		// 		{ .type = TOKEN_WORD, .data.word = "Makefile2" },
		// 	},
		// 	.token_length = 5,
		// 	.expected_ast = ast_new_node(AST_COMMAND,
		// 		ast_command_data(ft_split("cat", ' '), two_redir_list((t_redirection){.path = ft_strdup("Makefile"), .target_fd = 0, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile2"), .target_fd = 0, .redir_type = REDIR_INPUT})),
		// 		NULL, NULL)
		// },
		// {
		// 	.name = "cat < Makefile < Makefile2 123<Makefile3 -123<Makefile4",
		// 	.major_number = test_number,
		// 	.minor_number = 14,
		// 	.tokens = (t_token[]) {
		// 		{ .type = TOKEN_WORD, .data.word = "cat" },
		// 		{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
		// 		{ .type = TOKEN_WORD, .data.word = "Makefile" },
		// 		{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
		// 		{ .type = TOKEN_WORD, .data.word = "Makefile2" },
		// 		{ .type = TOKEN_REDIR_INPUT, .data.number = 123 },
		// 		{ .type = TOKEN_WORD, .data.word = "Makefile3" },
		// 		{ .type = TOKEN_WORD, .data.word = "-123" },
		// 		{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
		// 		{ .type = TOKEN_WORD, .data.word = "Makefile4" },
		// 	},
		// 	.token_length = 10,
		// 	.expected_ast = ast_new_node(AST_COMMAND,
		// 		ast_command_data(ft_split("cat -123", ' '), four_redir_list((t_redirection){.path = ft_strdup("Makefile"), .target_fd = 0, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile2"), .target_fd = 0, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile3"), .target_fd = 123, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile4"), .target_fd = 0, .redir_type = REDIR_INPUT})),
		// 		NULL, NULL)
		// },

		// CASE 5: ONLY REDIRS
		{
			.name = "< File",
			.major_number = test_number,
			.minor_number = 15,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_INPUT, 0, "File")
		},
		{
			.name = "> File",
			.major_number = test_number,
			.minor_number = 16,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_OUTPUT, 1, "File")
		},
		{
			.name = "<< EOF",
			.major_number = test_number,
			.minor_number = 17,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "EOF" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_HEREDOC, 0, "File")
		},
		{
			.name = ">> File",
			.major_number = test_number,
			.minor_number = 18,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_APPEND, 1, "File")
		},

		// CASE 6: ONLY REDIRS WITH FDs
		{
			.name = "99999< File",
			.major_number = test_number,
			.minor_number = 19,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = 99999 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_INPUT, 99999, "File")
		},
		{
			.name = "99999> File",
			.major_number = test_number,
			.minor_number = 20,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = 99999 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_OUTPUT, 99999, "File")
		},
		{
			.name = "99999<< EOF",
			.major_number = test_number,
			.minor_number = 21,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = 99999 },
				{ .type = TOKEN_WORD, .data.word = "EOF" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_HEREDOC, 99999, "File")
		},
		{
			.name = "99999>> File",
			.major_number = test_number,
			.minor_number = 22,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_APPEND, .data.number = 99999 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = TEST_REDIR(REDIR_APPEND, 99999, "File")
		},

		// CASE 7: EMPTY REDIRS
		{
			.name = "<",
			.major_number = test_number,
			.minor_number = 23,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = ">",
			.major_number = test_number,
			.minor_number = 24,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = -1 },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "<<",
			.major_number = test_number,
			.minor_number = 25,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = ">>",
			.major_number = test_number,
			.minor_number = 26,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
			},
			.token_length = 1,
			.expected_ast = NULL
		},

		// CASE 8: EMPTY REDIR WITH NEWLINE
		{
			.name = "< [newline]",
			.major_number = test_number,
			.minor_number = 27,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "> [newline]",
			.major_number = test_number,
			.minor_number = 28,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = -1 },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "<< [newline]",
			.major_number = test_number,
			.minor_number = 29,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = ">> [newline]",
			.major_number = test_number,
			.minor_number = 30,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},

		// CASE 9: EMPTY REDIR WITH OTHER TOKENS
		{
			.name = "<&&",
			.major_number = test_number,
			.minor_number = 31,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_IF_AND },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = ">||",
			.major_number = test_number,
			.minor_number = 32,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_OUTPUT, .data.number = -1 },
				{ .type = TOKEN_IF_OR },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "<<(",
			.major_number = test_number,
			.minor_number = 33,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
				{ .type = TOKEN_PAREN_LEFT },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = ">>)",
			.major_number = test_number,
			.minor_number = 34,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 2,
			.expected_ast = NULL
		},

		// CASE 10: PIPELINE OF SIMPLE COMMANDS (2 commands)
		{
			.name = "ls | cat",
			.major_number = test_number,
			.minor_number = 35,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
			},
			.token_length = 3,
			.expected_ast = TEST_PIPE(TEST_COMMAND("ls"), TEST_COMMAND("cat"))
		},
		{
			.name = "cat Makefile | sort -r",
			.major_number = test_number,
			.minor_number = 36,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_WORD, .data.word = "-r" },
			},
			.token_length = 5,
			.expected_ast = TEST_PIPE(TEST_COMMAND("cat Makefile"), TEST_COMMAND("sort -r"))
		},
		{
			.name = "cat file1 file2 file3 file4 file5 file6 file7 | sort -rrrrr -r -r -rrrr",
			.major_number = test_number,
			.minor_number = 37,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_WORD, .data.word = "file1" },
				{ .type = TOKEN_WORD, .data.word = "file2" },
				{ .type = TOKEN_WORD, .data.word = "file3" },
				{ .type = TOKEN_WORD, .data.word = "file4" },
				{ .type = TOKEN_WORD, .data.word = "file5" },
				{ .type = TOKEN_WORD, .data.word = "file6" },
				{ .type = TOKEN_WORD, .data.word = "file7" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_WORD, .data.word = "-rrrrr" },
				{ .type = TOKEN_WORD, .data.word = "-r" },
				{ .type = TOKEN_WORD, .data.word = "-r" },
				{ .type = TOKEN_WORD, .data.word = "-rrrr" },
			},
			.token_length = 14,
			.expected_ast = TEST_PIPE(TEST_COMMAND("cat file1 file2 file3 file4 file5 file6 file7"), TEST_COMMAND("sort -rrrrr -r -r -rrrr"))
		},

		// CASE 11: PIPELINE OF COMMANDS WITH REDIRECTIONS (2 commands)
		{
			.name = "cat < Makefile | sort",
			.major_number = test_number,
			.minor_number = 38,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
			},
			.token_length = 5,
			.expected_ast = TEST_PIPE(TEST_COMMAND_REDIR("cat", REDIR_INPUT, 0, "Makefile"), TEST_COMMAND("sort"))
		},
		{
			.name = "cat < Makefile | sort >> outfile",
			.major_number = test_number,
			.minor_number = 39,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_REDIR_APPEND, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "outfile" },
			},
			.token_length = 7,
			.expected_ast = TEST_PIPE(TEST_COMMAND_REDIR("cat", REDIR_INPUT, 0, "Makefile"), TEST_COMMAND_REDIR("sort", REDIR_APPEND, 1, "outfile"))
		},

		// CASE 12: LONG PIPELINES OF SIMPLE COMMANDS
		{
			.name = "ls | cat | sort",
			.major_number = test_number,
			.minor_number = 40,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
			},
			.token_length = 5,
			.expected_ast = TEST_PIPE(TEST_PIPE(TEST_COMMAND("ls"), TEST_COMMAND("cat")), TEST_COMMAND("sort"))
		},
		{
			.name = "ls | cat | sort | cat | sort | cat | sort | cat | sort | cat | sort | cat | sort | cat | sort | wc",
			.major_number = test_number,
			.minor_number = 41,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "sort" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "wc" },
			},
			.token_length = 31,
			.expected_ast = TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_COMMAND("ls"), TEST_COMMAND("cat")), TEST_COMMAND("sort")), TEST_COMMAND("cat")), TEST_COMMAND("sort")), TEST_COMMAND("cat")), TEST_COMMAND("sort")), TEST_COMMAND("cat")), TEST_COMMAND("sort")), TEST_COMMAND("cat")), TEST_COMMAND("sort")), TEST_COMMAND("cat")), TEST_COMMAND("sort")), TEST_COMMAND("cat")), TEST_COMMAND("sort")), TEST_COMMAND("wc"))
		},

		// CASE 14: PIPE ERRORS
		{
			.name = "|",
			.major_number = test_number,
			.minor_number = 42,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PIPE },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "ls |",
			.major_number = test_number,
			.minor_number = 43,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "ls | |",
			.major_number = test_number,
			.minor_number = 44,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "ls | | | | | | | | | | | | | | | | | | | | |",
			.major_number = test_number,
			.minor_number = 45,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 22,
			.expected_ast = NULL
		},
		{
			.name = "ls | &&",
			.major_number = test_number,
			.minor_number = 46,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_IF_AND },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "ls | ||",
			.major_number = test_number,
			.minor_number = 47,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_IF_OR },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "ls |&",
			.major_number = test_number,
			.minor_number = 48,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},

		#ifdef MINISHELL_BONUS
		// CASE 15: Simple ANDs
		{
			.name = "true && true",
			.major_number = test_number,
			.minor_number = 49,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "true" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "true" },
			},
			.token_length = 3,
			.expected_ast = TEST_AND(TEST_COMMAND("true"), TEST_COMMAND("true"))
		},
		{
			.name = "ls && echo test",
			.major_number = test_number,
			.minor_number = 50,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "echo" },
				{ .type = TOKEN_WORD, .data.word = "test" },
			},
			.token_length = 4,
			.expected_ast = TEST_AND(TEST_COMMAND("ls"), TEST_COMMAND("echo test"))
		},
		{
			.name = "cmd1 && cmd2 && cmd3",
			.major_number = test_number,
			.minor_number = 51,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
			},
			.token_length = 5,
			.expected_ast = TEST_AND(TEST_AND(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3"))
		},
		{
			.name = "cmd1 && cmd2 && cmd3 && cmd4 && cmd5 && cmd6 && cmd7 && cmd8",
			.major_number = test_number,
			.minor_number = 52,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd6" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd7" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd8" },
			},
			.token_length = 15,
			.expected_ast = TEST_AND(TEST_AND(TEST_AND(TEST_AND(TEST_AND(TEST_AND(TEST_AND(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")), TEST_COMMAND("cmd4")), TEST_COMMAND("cmd5")), TEST_COMMAND("cmd6")), TEST_COMMAND("cmd7")), TEST_COMMAND("cmd8"))
		},

		// CASE 16: Simple ORs
		{
			.name = "true || true",
			.major_number = test_number,
			.minor_number = 53,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "true" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "true" },
			},
			.token_length = 3,
			.expected_ast = TEST_OR(TEST_COMMAND("true"), TEST_COMMAND("true"))
		},
		{
			.name = "ls || echo test",
			.major_number = test_number,
			.minor_number = 54,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "echo" },
				{ .type = TOKEN_WORD, .data.word = "test" },
			},
			.token_length = 4,
			.expected_ast = TEST_OR(TEST_COMMAND("ls"), TEST_COMMAND("echo test"))
		},
		{
			.name = "cmd1 || cmd2 || cmd3",
			.major_number = test_number,
			.minor_number = 55,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
			},
			.token_length = 5,
			.expected_ast = TEST_OR(TEST_OR(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3"))
		},
		{
			.name = "cmd1 || cmd2 || cmd3 || cmd4 || cmd5 || cmd6 || cmd7 || cmd8",
			.major_number = test_number,
			.minor_number = 56,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd6" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd7" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd8" },
			},
			.token_length = 15,
			.expected_ast = TEST_OR(TEST_OR(TEST_OR(TEST_OR(TEST_OR(TEST_OR(TEST_OR(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")), TEST_COMMAND("cmd4")), TEST_COMMAND("cmd5")), TEST_COMMAND("cmd6")), TEST_COMMAND("cmd7")), TEST_COMMAND("cmd8"))
		},

		// CASE 17: ANDs and ORs
		{
			.name = "cmd1 && cmd2 || cmd3",
			.major_number = test_number,
			.minor_number = 57,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
			},
			.token_length = 5,
			.expected_ast = TEST_OR(TEST_AND(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3"))
		},
		{
			.name = "cmd1 || cmd2 && cmd3",
			.major_number = test_number,
			.minor_number = 58,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
			},
			.token_length = 5,
			.expected_ast = TEST_AND(TEST_OR(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3"))
		},
		{
			.name = "cmd1 && cmd2 && cmd3 || cmd4 && cmd5",
			.major_number = test_number,
			.minor_number = 59,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
			},
			.token_length = 9,
			.expected_ast = TEST_AND(TEST_OR(TEST_AND(TEST_AND(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")), TEST_COMMAND("cmd4")), TEST_COMMAND("cmd5"))
		},
		{
			.name = "cmd1 && cmd2 || cmd3 && cmd4 && cmd5 || cmd6 && cmd7 || cmd8 && cmd9 && cmd10 || cmd11",
			.major_number = test_number,
			.minor_number = 60,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd6" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd7" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd8" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd9" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd10" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd11" },
			},
			.token_length = 21,
			.expected_ast = TEST_OR(TEST_AND(TEST_AND(TEST_OR(TEST_AND(TEST_OR(TEST_AND(TEST_AND(TEST_OR(TEST_AND(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")), TEST_COMMAND("cmd4")), TEST_COMMAND("cmd5")), TEST_COMMAND("cmd6")), TEST_COMMAND("cmd7")), TEST_COMMAND("cmd8")), TEST_COMMAND("cmd9")), TEST_COMMAND("cmd10")), TEST_COMMAND("cmd11"))
		},

		// CASE 17: && and || errors
		{
			.name = "&&",
			.major_number = test_number,
			.minor_number = 61,
			.tokens = (t_token[]) {
				{ .type = TOKEN_IF_AND },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "&&\\n",
			.major_number = test_number,
			.minor_number = 62,
			.tokens = (t_token[]) {
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "||",
			.major_number = test_number,
			.minor_number = 63,
			.tokens = (t_token[]) {
				{ .type = TOKEN_IF_OR },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "||\\n",
			.major_number = test_number,
			.minor_number = 64,
			.tokens = (t_token[]) {
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "cmd &&",
			.major_number = test_number,
			.minor_number = 65,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_AND },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "cmd &&\\n",
			.major_number = test_number,
			.minor_number = 66,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "cmd ||",
			.major_number = test_number,
			.minor_number = 67,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_OR },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "cmd ||\\n",
			.major_number = test_number,
			.minor_number = 68,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_NEWLINE },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "cmd && &&",
			.major_number = test_number,
			.minor_number = 69,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_AND },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "cmd && ||",
			.major_number = test_number,
			.minor_number = 70,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_IF_OR },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "cmd || ||",
			.major_number = test_number,
			.minor_number = 71,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_OR },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "cmd || &&",
			.major_number = test_number,
			.minor_number = 72,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_IF_AND },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "cmd || |",
			.major_number = test_number,
			.minor_number = 73,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "cmd && |",
			.major_number = test_number,
			.minor_number = 74,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 3,
			.expected_ast = NULL
		},

		// CASE 18: Single Level 1 Subshell
		{
			.name = "(cmd)",
			.major_number = test_number,
			.minor_number = 75,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 5,
			.expected_ast = TEST_SUBSHELL(TEST_COMMAND("cmd"))
		},
		{
			.name = "(cmd1 | cmd2)",
			.major_number = test_number,
			.minor_number = 76,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 5,
			.expected_ast = TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")))
		},
		{
			.name = "(cmd1 | cmd2 | cmd3 | cmd4 | cmd5 | cmd6)",
			.major_number = test_number,
			.minor_number = 77,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd6" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 13,
			.expected_ast = TEST_SUBSHELL(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_PIPE(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")), TEST_COMMAND("cmd4")), TEST_COMMAND("cmd5")), TEST_COMMAND("cmd6")))
		},
		{
			.name = "(cmd1 && cmd2)",
			.major_number = test_number,
			.minor_number = 78,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 5,
			.expected_ast = TEST_SUBSHELL(TEST_AND(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")))
		},
		{
			.name = "(cmd1 || cmd2)",
			.major_number = test_number,
			.minor_number = 79,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 5,
			.expected_ast = TEST_SUBSHELL(TEST_OR(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")))
		},
		{
			.name = "(cmd1 && cmd2 || cmd3)",
			.major_number = test_number,
			.minor_number = 80,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 7,
			.expected_ast = TEST_SUBSHELL(TEST_OR(TEST_AND(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")))
		},
		{
			.name = "(cmd1 || cmd2 && cmd3)",
			.major_number = test_number,
			.minor_number = 81,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 7,
			.expected_ast = TEST_SUBSHELL(TEST_AND(TEST_OR(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")))
		},
		{
			.name = "(cmd1 || cmd2 && cmd3 && cmd4 || cmd5 && cmd6 && cmd7 || cmd8)",
			.major_number = test_number,
			.minor_number = 82,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd6" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "cmd7" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "cmd8" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 17,
			.expected_ast = TEST_SUBSHELL(TEST_OR(TEST_AND(TEST_AND(TEST_OR(TEST_AND(TEST_AND(TEST_OR(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2")), TEST_COMMAND("cmd3")), TEST_COMMAND("cmd4")), TEST_COMMAND("cmd5")), TEST_COMMAND("cmd6")), TEST_COMMAND("cmd7")), TEST_COMMAND("cmd8")))
		},

		// CASE 19: Multiple level 1 subshells
		{
			.name = "(cmd1) | (cmd2)",
			.major_number = test_number,
			.minor_number = 83,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 7,
			.expected_ast = TEST_PIPE(TEST_SUBSHELL(TEST_COMMAND("cmd1")), TEST_SUBSHELL(TEST_COMMAND("cmd2")))
		},
		{
			.name = "(cmd1 | cmd2) | (cmd3 | cmd4)",
			.major_number = test_number,
			.minor_number = 84,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 11,
			.expected_ast = TEST_PIPE(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd3"), TEST_COMMAND("cmd4"))))
		},
		{
			.name = "(cmd1 | cmd2) && (cmd3 | cmd4)",
			.major_number = test_number,
			.minor_number = 85,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 11,
			.expected_ast = TEST_AND(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd3"), TEST_COMMAND("cmd4"))))
		},
		{
			.name = "(cmd1 | cmd2) || (cmd3 | cmd4)",
			.major_number = test_number,
			.minor_number = 86,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 11,
			.expected_ast = TEST_OR(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd3"), TEST_COMMAND("cmd4"))))
		},
		{
			.name = "(cmd1 | cmd2) | (cmd3 | cmd4) && (cmd5 | cmd6)",
			.major_number = test_number,
			.minor_number = 87,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd6" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 17,
			.expected_ast = TEST_AND(TEST_PIPE(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd3"), TEST_COMMAND("cmd4")))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd5"), TEST_COMMAND("cmd6"))))
		},
		{
			.name = "(cmd1 | cmd2) | (cmd3 | cmd4) && (cmd5 | cmd6) || (cmd7 | cmd8) | (cmd9 | cmd10)",
			.major_number = test_number,
			.minor_number = 88,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd5" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd6" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd7" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd8" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd9" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "cmd10" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 29,
			.expected_ast = TEST_OR(TEST_AND(TEST_PIPE(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd1"), TEST_COMMAND("cmd2"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd3"), TEST_COMMAND("cmd4")))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd5"), TEST_COMMAND("cmd6")))), TEST_PIPE(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd7"), TEST_COMMAND("cmd8"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("cmd9"), TEST_COMMAND("cmd10")))))
		},

		// CASE 20: Single nested subshell with only one command
		{
			.name = "((cmd1))",
			.major_number = test_number,
			.minor_number = 89,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 5,
			.expected_ast = TEST_SUBSHELL(TEST_SUBSHELL(TEST_COMMAND("cmd1")))
		},
		{
			.name = "(((cmd1)))",
			.major_number = test_number,
			.minor_number = 90,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 7,
			.expected_ast = TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_COMMAND("cmd1"))))
		},
		{
			.name = "((((((((((cmd1 arg1 arg2 arg3 arg4))))))))))",
			.major_number = test_number,
			.minor_number = 91,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_WORD, .data.word = "arg1" },
				{ .type = TOKEN_WORD, .data.word = "arg2" },
				{ .type = TOKEN_WORD, .data.word = "arg3" },
				{ .type = TOKEN_WORD, .data.word = "arg4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 27,
			.expected_ast = TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_SUBSHELL(TEST_COMMAND("cmd1 arg1 arg2 arg3 arg4")))))))))))
		},

		// CASE 21: Single nested subshells with multiple commands and operators
		{
			.name = "((cmd1) | (cmd2))",
			.major_number = test_number,
			.minor_number = 92,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 9,
			.expected_ast = TEST_SUBSHELL(TEST_PIPE(TEST_SUBSHELL(TEST_COMMAND("cmd1")), TEST_SUBSHELL(TEST_COMMAND("cmd2"))))
		},
		{
			.name = "((cmd1) && (cmd2))",
			.major_number = test_number,
			.minor_number = 93,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 9,
			.expected_ast = TEST_SUBSHELL(TEST_AND(TEST_SUBSHELL(TEST_COMMAND("cmd1")), TEST_SUBSHELL(TEST_COMMAND("cmd2"))))
		},
		{
			.name = "((cmd1) || (cmd2))",
			.major_number = test_number,
			.minor_number = 94,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 9,
			.expected_ast = TEST_SUBSHELL(TEST_OR(TEST_SUBSHELL(TEST_COMMAND("cmd1")), TEST_SUBSHELL(TEST_COMMAND("cmd2"))))
		},
		{
			.name = "(1 && (2 || 3))",
			.major_number = test_number,
			.minor_number = 95,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "3" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 9,
			.expected_ast = TEST_SUBSHELL(TEST_AND(TEST_COMMAND("1"), TEST_SUBSHELL(TEST_OR(TEST_COMMAND("2"), TEST_COMMAND("3")))))
		},
		{
			.name = "(1 || (2 && 3))",
			.major_number = test_number,
			.minor_number = 96,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "2" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "3" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 9,
			.expected_ast = TEST_SUBSHELL(TEST_OR(TEST_COMMAND("1"), TEST_SUBSHELL(TEST_AND(TEST_COMMAND("2"), TEST_COMMAND("3")))))
		},
		{
			.name = "((1 | 2) && 3)",
			.major_number = test_number,
			.minor_number = 97,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "3" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 9,
			.expected_ast = TEST_SUBSHELL(TEST_AND(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("1"), TEST_COMMAND("2"))), TEST_COMMAND("3")))
		},
		{
			.name = "(1 && (2 || (3 && 4)))",
			.major_number = test_number,
			.minor_number = 98,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "3" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 13,
			.expected_ast = TEST_SUBSHELL(TEST_AND(TEST_COMMAND("1"), TEST_SUBSHELL(TEST_OR(TEST_COMMAND("2"), TEST_SUBSHELL(TEST_AND(TEST_COMMAND("3"), TEST_COMMAND("4")))))))
		},
		{
			.name = "((1 || 2) && (3 | 4))",
			.major_number = test_number,
			.minor_number = 99,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "1" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "2" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "3" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 13,
			.expected_ast = TEST_SUBSHELL(TEST_AND(TEST_SUBSHELL(TEST_OR(TEST_COMMAND("1"), TEST_COMMAND("2"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("3"), TEST_COMMAND("4")))))
		},
		{
			.name = "((1 | 2) || (3 && 4))",
			.major_number = test_number,
			.minor_number = 100,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "1" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "3" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "4" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 13,
			.expected_ast = TEST_SUBSHELL(TEST_OR(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("1"), TEST_COMMAND("2"))), TEST_SUBSHELL(TEST_AND(TEST_COMMAND("3"), TEST_COMMAND("4")))))
		},
		{
			.name = "(1 && (2 || 3) && ((4 | 5) || (6 && 7)) || (8 && (9 | (10 || 11))) && ((12 || 13) && (14 | 15)) || ((16 && 17) || (18 | (19 && 20))))",
			.major_number = test_number,
			.minor_number = 100,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "1" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "2" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "3" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "4" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "5" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "6" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "7" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "8" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "9" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "10" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "11" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "12" },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_WORD, .data.word = "13" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "14" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_WORD, .data.word = "15" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "16" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "17" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "18" },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "19" },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_WORD, .data.word = "20" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 69,
			.expected_ast = TEST_SUBSHELL(TEST_OR(TEST_AND(TEST_OR(TEST_AND(TEST_AND(TEST_COMMAND("1"), TEST_SUBSHELL(TEST_OR(TEST_COMMAND("2"), TEST_COMMAND("3")))), TEST_SUBSHELL(TEST_OR(TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("4"), TEST_COMMAND("5"))), TEST_SUBSHELL(TEST_AND(TEST_COMMAND("6"), TEST_COMMAND("7")))))), TEST_SUBSHELL(TEST_AND(TEST_COMMAND("8"), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("9"), TEST_SUBSHELL(TEST_OR(TEST_COMMAND("10"), TEST_COMMAND("11")))))))), TEST_SUBSHELL(TEST_AND(TEST_SUBSHELL(TEST_OR(TEST_COMMAND("12"), TEST_COMMAND("13"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("14"), TEST_COMMAND("15")))))), TEST_SUBSHELL(TEST_OR(TEST_SUBSHELL(TEST_AND(TEST_COMMAND("16"), TEST_COMMAND("17"))), TEST_SUBSHELL(TEST_PIPE(TEST_COMMAND("18"), TEST_SUBSHELL(TEST_AND(TEST_COMMAND("19"), TEST_COMMAND("20")))))))))
		},
		// CASE 22: Subshell errors
		{
			.name = "(",
			.major_number = test_number,
			.minor_number = 101,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = ")",
			.major_number = test_number,
			.minor_number = 102,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "()",
			.major_number = test_number,
			.minor_number = 103,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "((((((((((",
			.major_number = test_number,
			.minor_number = 104,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
			},
			.token_length = 10,
			.expected_ast = NULL
		},
		{
			.name = "))))))))))",
			.major_number = test_number,
			.minor_number = 105,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 10,
			.expected_ast = NULL
		},
		{
			.name = "(((((((((()))))))))",
			.major_number = test_number,
			.minor_number = 106,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 20,
			.expected_ast = NULL
		},
		{
			.name = "(&&",
			.major_number = test_number,
			.minor_number = 107,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_IF_AND },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "(||",
			.major_number = test_number,
			.minor_number = 108,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_IF_OR },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "(|",
			.major_number = test_number,
			.minor_number = 109,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "(&&)",
			.major_number = test_number,
			.minor_number = 110,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_IF_AND },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "(||)",
			.major_number = test_number,
			.minor_number = 111,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_IF_OR },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "(|)",
			.major_number = test_number,
			.minor_number = 112,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PIPE },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "(<)",
			.major_number = test_number,
			.minor_number = 113,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_REDIR_INPUT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "(>)",
			.major_number = test_number,
			.minor_number = 114,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_REDIR_OUTPUT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "(<<)",
			.major_number = test_number,
			.minor_number = 115,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_REDIR_HEREDOC },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "(>>)",
			.major_number = test_number,
			.minor_number = 116,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_REDIR_APPEND },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 3,
			.expected_ast = NULL
		},
		{
			.name = "(cmd1",
			.major_number = test_number,
			.minor_number = 117,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
		{
			.name = "(cmd1))",
			.major_number = test_number,
			.minor_number = 118,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 4,
			.expected_ast = NULL
		},
		{
			.name = "(((((cmd1",
			.major_number = test_number,
			.minor_number = 119,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
			},
			.token_length = 6,
			.expected_ast = NULL
		},
		{
			.name = "(((((cmd1))))",
			.major_number = test_number,
			.minor_number = 120,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_PAREN_RIGHT },

			},
			.token_length = 10,
			.expected_ast = NULL
		},
		{
			.name = "((cmd1) cmd2",
			.major_number = test_number,
			.minor_number = 121,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
			},
			.token_length = 5,
			.expected_ast = NULL
		},
		{
			.name = "((cmd1) cmd2)",
			.major_number = test_number,
			.minor_number = 122,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 6,
			.expected_ast = NULL
		},
		{
			.name = "(cmd1 (cmd2) cmd3",
			.major_number = test_number,
			.minor_number = 123,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
			},
			.token_length = 6,
			.expected_ast = NULL
		},
		{
			.name = "(cmd1 (cmd2) cmd3)",
			.major_number = test_number,
			.minor_number = 124,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd1" },
				{ .type = TOKEN_PAREN_LEFT },
				{ .type = TOKEN_WORD, .data.word = "cmd2" },
				{ .type = TOKEN_PAREN_RIGHT },
				{ .type = TOKEN_WORD, .data.word = "cmd3" },
				{ .type = TOKEN_PAREN_RIGHT },
			},
			.token_length = 7,
			.expected_ast = NULL
		},
		// this one is the exact same copy of the one above, except one random part
		// is removed to test parser errors
		// {
		// 	.name = "(1 && (2 || 3) && ((4 | 5) || (6 && 7)) || (8 && (9 | (10 || 11))) && ((12 || 13) && (14 | 15)) || ((16 && 17) || (18 | (19 && 20))))",
		// 	.major_number = test_number,
		// 	.minor_number = 100,
		// 	.tokens = (t_token[]) {
		// 		//{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "1" },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "2" },
		// 		{ .type = TOKEN_IF_OR },
		// 		{ .type = TOKEN_WORD, .data.word = "3" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "4" },
		// 		{ .type = TOKEN_PIPE },
		// 		{ .type = TOKEN_WORD, .data.word = "5" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_IF_OR },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "6" },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_WORD, .data.word = "7" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_IF_OR },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "8" },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "9" },
		// 		{ .type = TOKEN_PIPE },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "10" },
		// 		{ .type = TOKEN_IF_OR },
		// 		{ .type = TOKEN_WORD, .data.word = "11" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "12" },
		// 		{ .type = TOKEN_IF_OR },
		// 		{ .type = TOKEN_WORD, .data.word = "13" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "14" },
		// 		{ .type = TOKEN_PIPE },
		// 		{ .type = TOKEN_WORD, .data.word = "15" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_IF_OR },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "16" },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_WORD, .data.word = "17" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_IF_OR },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "18" },
		// 		{ .type = TOKEN_PIPE },
		// 		{ .type = TOKEN_PAREN_LEFT },
		// 		{ .type = TOKEN_WORD, .data.word = "19" },
		// 		{ .type = TOKEN_IF_AND },
		// 		{ .type = TOKEN_WORD, .data.word = "20" },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 		{ .type = TOKEN_PAREN_RIGHT },
		// 	},
		// 	.token_length = 68,
		// 	.expected_ast = NULL
		// },
		#endif
	};

	t_test_result results;

	printf("\nTest %i: Testing parser() function\n", test_number);

	for (size_t i = 0; i < sizeof(tests) / sizeof(t_parser_test); i++)
	{
		results = conduct_parser_test(tests[i]);
	}

	printf("\n==================[RESULTS]==================\n\n");
	printf(COLOR_GREEN "PASS: " COLOR_WHITE "%i/%i\n", results.passed, results.total);
	printf(COLOR_RED "FAIL: " COLOR_WHITE "%i/%i\n", results.failed, results.total);
	printf("\n[TEST %i DONE]\n", test_number);
}

int	main(void)
{
	printf("\nPARSER TESTER\n");

	test_parser(1);
	return (EXIT_SUCCESS);
}
