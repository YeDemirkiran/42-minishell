/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_traversal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:17:47 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/28 22:41:20 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <structs/s_ast.h>
#include <stddef.h>

// Traverses the AST with pre-order algorithm and applies FUNC
void	preorder_traverse_ast(t_ast *ast, void (*f)(t_ast *))
{
	if (ast == NULL || f == NULL)
		return ;
	f(ast);
	if (ast->left != NULL)
		preorder_traverse_ast(ast->left, f);
	if (ast->right != NULL)
		preorder_traverse_ast(ast->right, f);
}

// Traverses the AST with post-order algorithm and applies FUNC
void	postorder_traverse_ast(t_ast *ast, void (*f)(t_ast *))
{
	if (ast == NULL || f == NULL)
		return ;
	if (ast->left != NULL)
		postorder_traverse_ast(ast->left, f);
	if (ast->right != NULL)
		postorder_traverse_ast(ast->right, f);
	f(ast);
}
