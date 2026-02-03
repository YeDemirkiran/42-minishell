/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conditional_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 19:46:11 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:30:02 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Prohjeect headers
#include "structs/s_shell.h"
#include "modules/ast.h"

// Local headers
#include "parse_result.h"
#include "parse_pipeline.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Calls ast_free_tree on both left and right, if they are not NULL.
 * @return The result parameter.
 */
static t_parse_result	free_and_return(t_ast *left, t_ast *right,
	t_parse_result result)
{
	if (left != NULL)
		ast_free_tree(left);
	if (right != NULL)
		ast_free_tree(right);
	return (result);
}

static t_parse_result	parse_and_or(t_list *token, t_ast *left,
	t_shell shell, t_ast_type type)
{
	t_parse_result	result;
	t_ast			*node;

	if (token->next == NULL || token->next->content == NULL)
		return (free_and_return(left, NULL, parse_uncomplete_error(token)));
	result = parse_pipeline(token->next, shell);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (free_and_return(left, NULL, result));
	else if (result.ast == NULL)
		return (free_and_return(left, NULL, parse_syntax_error(token->next)));
	token = result.current_token;
	node = ast_new_node(type, NULL, left, result.ast);
	if (node == NULL)
		return (free_and_return(left, result.ast, parse_runtime_error(token)));
	return (parse_success(node, token));
}

t_parse_result	parse_conditional(t_list *token, t_shell shell)
{
	t_ast			*left;
	t_parse_result	result;
	t_token_type	type;

	result = parse_pipeline(token, shell);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	left = result.ast;
	token = result.current_token;
	while (token != NULL)
	{
		type = ((t_token *)(token->content))->type;
		if (type == TOKEN_IF_AND)
			result = parse_and_or(token, left, shell, AST_IF);
		else if (type == TOKEN_IF_OR)
			result = parse_and_or(token, left, shell, AST_OR);
		else
			break ;
		if (result.type != PARSE_RESULT_SUCCESS)
			return (result);
		left = result.ast;
		token = result.current_token;
	}
	return (parse_success(left, token));
}

/* ================================== END ================================== */
