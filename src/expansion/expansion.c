/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/02 10:59:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

#include "structs/s_ast.h"
#include "modules/ast.h"

#include "variable_expansion.h"
#include "tilde_expansion.h"
#include "split_word.h"
#include "remove_quotes.h"

#ifdef MINISHELL_BONUS
# include "filename_expansion_bonus.h"
#endif

//TODO: Remove
/*static void	print_command_args(t_ast *ast)
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
}*/

#ifdef MINISHELL_BONUS

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	if (perform_tilde_expansion(root, shell) != 1)
		return (0);
	if (perform_variable_expansion(root, shell) != 1)
		return (0);
	if (perform_word_split(root, shell) != 1)
		return (0);
	if (perform_filename_expansion(root) != 1)
		return (0);
	perform_quote_removal(root);
	return (1);
}

#else

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	if (perform_tilde_expansion(root, shell) != 1)
		return (0);
	if (perform_variable_expansion(root, shell) != 1)
		return (0);
	if (perform_word_split(root, shell) != 1)
		return (0);
	perform_quote_removal(root);
	return (1);
}

#endif
