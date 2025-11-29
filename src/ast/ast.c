/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:45:56 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/29 16:35:48 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <structs/s_ast.h>
#include <structs/s_redirection.h>
#include <modules/ast.h>

// Initializes and returns a data union with command struct values.
t_ast_data	*ast_command_data(char *command, char **args, t_list *redirs)
{
	t_ast_data	*data;

	data = malloc(sizeof(t_ast_data));
	if (data == NULL)
		return (NULL);
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

// Takes a void pointer, casts it to t_redirection* and frees its content.
static void	free_redir_struct(void *redir)
{
	free(((t_redirection *)redir)->path);
}

// Frees the node without traversal.
static void	ast_free_node(t_ast *ast)
{
	size_t	i;

	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
	{
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
		ft_lstclear(&(ast->data->ast_command.redirs), free_redir_struct);
	}
	free(ast);
}

// Recursively frees node and its children ith the DFS post-order algorithm.
void	ast_free_tree(t_ast *ast)
{
	ast_traverse_postorder(ast, ast_free_node);
}
