/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:14:20 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/20 13:58:43 by yademirk         ###   ########.fr       */
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

#include <modules/parser.h>
#include <modules/ast.h>

#define COLOR_GREEN "\e[0;32m"
#define COLOR_RED "\e[0;31m"
#define COLOR_WHITE "\e[0;37m"

#define TEST_COMMAND(str) ast_new_node(AST_COMMAND, ast_command_data(ft_split(str, ' '), NULL), NULL, NULL)
#define TEST_COMMAND_REDIR(str, redir_type, redir_fd, redir_path) ast_new_node(AST_COMMAND, ast_command_data(ft_split(str, ' '), ft_lstnew(alloc_redir(redir_type, redir_fd, ft_strdup(redir_path)))), NULL, NULL)
#define TEST_PIPE(left, right) ast_new_node(AST_PIPE, NULL, left, right)

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
		char	**expected_args = expected->data->ast_command.arguments;
		char	**testee_args = testee->data->ast_command.arguments;


		// NULL check
		if ((expected_args == NULL) != (testee_args == NULL))
			return (0);

		// Arguments
		if (expected_args != NULL)
		{
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

	printf("Test %i.%i: %s\n", test.major_number, test.minor_number, test.name);

	printf("Calling parse()...\n");
	printf("=============================================================\n");
	list = token_list_to_linked_list(test.tokens, test.token_length);
	retrieved_ast = parse(list);
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

static t_list	*two_redir_list(t_redirection first, t_redirection second)
{
	t_list	*root;

	root = ft_lstnew(alloc_redir(first.redir_type, first.target_fd, first.path));
	root->next = ft_lstnew(alloc_redir(second.redir_type, second.target_fd, second.path));
	return (root);
}

static t_list	*four_redir_list(t_redirection first, t_redirection second, t_redirection third, t_redirection fourth)
{
	t_list	*root;

	root = two_redir_list(first, second);
	root->next->next = two_redir_list(third, fourth);
	return (root);
}

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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("ls", ' '), NULL),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("ls -l -a", ' '), NULL),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat Makefile README.md this_file_doesnt_exist_lol BROTHER_HOW Im_running_out_of_shit_to_test", ' '), NULL),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_INPUT, 0, ft_strdup("Makefile")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_HEREDOC, 0, ft_strdup("EOF")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_OUTPUT, 1, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_APPEND, 1, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_INPUT, 999999999, ft_strdup("Makefile")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_HEREDOC, 999999999, ft_strdup("EOF")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_OUTPUT, 999999999, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), ft_lstnew(alloc_redir(REDIR_APPEND, 999999999, ft_strdup("File")))),
				NULL, NULL)
		},

		// CASE 4: MULTIPLE REDIRECTIONS
		{
			.name = "cat < Makefile < Makefile2",
			.major_number = test_number,
			.minor_number = 13,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile2" },
			},
			.token_length = 5,
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat", ' '), two_redir_list((t_redirection){.path = ft_strdup("Makefile"), .target_fd = 0, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile2"), .target_fd = 0, .redir_type = REDIR_INPUT})),
				NULL, NULL)
		},
		{
			.name = "cat < Makefile < Makefile2 123<Makefile3 -123<Makefile4",
			.major_number = test_number,
			.minor_number = 14,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "cat" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile2" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = 123 },
				{ .type = TOKEN_WORD, .data.word = "Makefile3" },
				{ .type = TOKEN_WORD, .data.word = "-123" },
				{ .type = TOKEN_REDIR_INPUT, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "Makefile4" },
			},
			.token_length = 10,
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(ft_split("cat -123", ' '), four_redir_list((t_redirection){.path = ft_strdup("Makefile"), .target_fd = 0, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile2"), .target_fd = 0, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile3"), .target_fd = 123, .redir_type = REDIR_INPUT}, (t_redirection){.path = ft_strdup("Makefile4"), .target_fd = 0, .redir_type = REDIR_INPUT})),
				NULL, NULL)
		},

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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_INPUT, 0, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_OUTPUT, 1, ft_strdup("File")))),
				NULL, NULL)
		},
		{
			.name = "<< File",
			.major_number = test_number,
			.minor_number = 17,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = -1 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_HEREDOC, 0, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_APPEND, 1, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_INPUT, 99999, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_OUTPUT, 99999, ft_strdup("File")))),
				NULL, NULL)
		},
		{
			.name = "99999<< File",
			.major_number = test_number,
			.minor_number = 21,
			.tokens = (t_token[]) {
				{ .type = TOKEN_REDIR_HEREDOC, .data.number = 99999 },
				{ .type = TOKEN_WORD, .data.word = "File" },
			},
			.token_length = 2,
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_HEREDOC, 99999, ft_strdup("File")))),
				NULL, NULL)
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
			.expected_ast = ast_new_node(AST_COMMAND,
				ast_command_data(NULL, ft_lstnew(alloc_redir(REDIR_APPEND, 99999, ft_strdup("File")))),
				NULL, NULL)
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
			.minor_number = 40,
			.tokens = (t_token[]) {
				{ .type = TOKEN_PIPE },
			},
			.token_length = 1,
			.expected_ast = NULL
		},
		{
			.name = "ls |",
			.major_number = test_number,
			.minor_number = 40,
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
			.minor_number = 40,
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
			.minor_number = 40,
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
			.minor_number = 40,
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
			.minor_number = 40,
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
			.minor_number = 40,
			.tokens = (t_token[]) {
				{ .type = TOKEN_WORD, .data.word = "ls" },
				{ .type = TOKEN_PIPE },
			},
			.token_length = 2,
			.expected_ast = NULL
		},
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
