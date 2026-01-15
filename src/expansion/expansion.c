/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/15 13:28:34 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#include "structs/s_ast.h"
#include "modules/ast.h"

#include "variable_expansion.h"
#include "tilde_expansion.h"
#include "split_word.h"
#include "remove_quotes.h"

//TODO: Remove
static void	print_command_args(t_ast *ast)
{
	t_list	*lst;

	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
		return ;
	lst = ast->data->ast_command.arguments;
	if (lst == NULL)
		return ;
	printf("CMD: %s, Args: ", (char *)(lst->content));
	while (lst != NULL)
	{
		printf("%s", (char *)(lst->content));
		if (lst->next != NULL)
			printf(", ");
		lst = lst->next;
	}
	printf("\n");
}

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	// printf("\n(DEBUG: Starting shell step 3: Expansion)\n");
	// printf("(DEBUG: Step 3.1: Variable Substiton)\n");
	if (perform_tilde_expansion(root, &(shell.env->envp_list)) != 1)
		return (0);
	if (perform_variable_expansion(root, shell) != 1)
		return (0);
	//printf("(DEBUG: Step 3.2: Word Splitting)\n");
	if (perform_word_split(root) != 1)
		return (0);
	//printf("(DEBUG: Step 3.3: Quote Removal)\n");
	perform_quote_removal(root);
	printf("(DEBUG: Step 3.4: Printing New Variables)\n");
	ast_traverse_preorder(root, print_command_args);
	return (1);
}
