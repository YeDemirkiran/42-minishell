/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_traversal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:17:47 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/17 13:51:45 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stddef.h>

// Project headers
#include "structs/s_ast.h"

/* =============================== FUNCTIONS =============================== */

// Traverses the AST with pre-order algorithm and applies FUNC
void	ast_traverse_preorder(t_ast *ast, void (*f)(t_ast *))
{
	if (ast == NULL || f == NULL)
		return ;
	f(ast);
	if (ast->left != NULL)
		ast_traverse_preorder(ast->left, f);
	if (ast->right != NULL)
		ast_traverse_preorder(ast->right, f);
}

// Traverses the AST with post-order algorithm and applies FUNC
void	ast_traverse_postorder(t_ast *ast, void (*f)(t_ast *))
{
	if (ast == NULL || f == NULL)
		return ;
	if (ast->left != NULL)
		ast_traverse_postorder(ast->left, f);
	if (ast->right != NULL)
		ast_traverse_postorder(ast->right, f);
	f(ast);
}

/* ================================== END ================================== */
