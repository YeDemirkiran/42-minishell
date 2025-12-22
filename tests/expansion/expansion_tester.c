/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_tester.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:14:20 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/22 16:39:34 by yademirk         ###   ########.fr       */
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
#define TEST_PIPE(left, right) ast_new_node(AST_PIPE, NULL, left, right)

int	main(void)
{
	printf("\nEXPANSION TESTER\n");

	test_parser(1);
	return (EXIT_SUCCESS);
}
