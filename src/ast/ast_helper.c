/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:27:52 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/13 17:32:19 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>

// Project headers
#include "structs/s_ast.h"
#include "structs/s_redirection.h"

/* =============================== FUNCTIONS =============================== */

// Takes a void pointer, casts it to t_redirection* and frees its content.
void	free_redir_struct(void *redir)
{
	t_redirection	*redir_obj;

	if (redir == NULL)
		return ;
	redir_obj = (t_redirection *)redir;
	if (redir_obj->path != NULL)
		free(redir_obj->path);
	free(redir_obj);
}

/**
 * @brief Frees an AST node's data only if it's of type AST_COMMAND.
 */
void	ast_free_command(t_ast *ast)
{
	size_t	i;

	if (ast->type != AST_COMMAND || ast->data == NULL)
		return ;
	if (ast->data->ast_command.arguments != NULL)
	{
		if (ast->data->ast_command.command != NULL)
			if (ast->data->ast_command.command
				!= ast->data->ast_command.arguments[0])
				free(ast->data->ast_command.command);
		i = 0;
		while (ast->data->ast_command.arguments[i] != NULL)
		{
			free(ast->data->ast_command.arguments[i]);
			i++;
		}
		free(ast->data->ast_command.arguments);
	}
	else if (ast->data->ast_command.command != NULL)
	{
		free(ast->data->ast_command.command);
	}
	ft_lstclear(&(ast->data->ast_command.redirs), free_redir_struct);
}

/* ================================== END ================================== */
