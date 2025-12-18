/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:45:56 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 16:26:47 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>

// Project headers
#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "modules/ast.h"

// Local headers
#include "ast_helper.h"

/* =============================== FUNCTIONS =============================== */

// Initializes and returns a data union with command struct values.
t_ast_data	*ast_command_data(char **args, t_list *redirs)
{
	t_ast_data	*data;
	char		*command;

	data = malloc(sizeof(t_ast_data));
	if (data == NULL)
		return (NULL);
	if (args != NULL)
		command = args[0];
	else
		command = NULL;
	data->ast_command.command = command;
	data->ast_command.arguments = args;
	data->ast_command.redirs = redirs;
	return (data);
}

// Allocates memory for an AST node and returns it.
t_ast	*ast_new_node(t_ast_type type, t_ast_data *data,
	t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->data = data;
	node->left = left;
	node->right = right;
	return (node);
}

// Frees the node without traversal.
static void	ast_free_node(t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
		ast_free_command(ast);
	if (ast->data != NULL)
		free(ast->data);
	free(ast);
}

// Recursively frees node and its children ith the DFS post-order algorithm.
void	ast_free_tree(t_ast *ast)
{
	ast_traverse_postorder(ast, ast_free_node);
}

/* ================================== END ================================== */
