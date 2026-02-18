/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:36:50 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:16:13 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stddef.h>

// Project headers
#include "modules/ast.h"
#include "parse_command.h"

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

/**
 * @brief Parses a single pipe of two commands.
 */
static t_parse_result	parse_pipe(t_list *token, t_ast *left, t_shell shell)
{
	t_ast			*node;
	t_parse_result	result;

	if (token->next == NULL || token->next->content == NULL)
		return (free_and_return(left, NULL, parse_uncomplete_error(token)));
	result = parse_command(token->next, shell);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (free_and_return(left, NULL, result));
	else if (result.ast == NULL)
		return (free_and_return(left, NULL, parse_syntax_error(token->next)));
	token = result.current_token;
	node = ast_new_node(AST_PIPE, NULL, left, result.ast);
	if (node == NULL)
		return (free_and_return(left, result.ast, parse_runtime_error(token)));
	return (parse_success(node, token));
}

/**
 * @brief Parses a pipeline.
 */
t_parse_result	parse_pipeline(t_list *token, t_shell shell)
{
	t_parse_result	result;
	t_ast			*left;

	result = parse_command(token, shell);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	left = result.ast;
	token = result.current_token;
	while (token != NULL && token->content != NULL)
	{
		if (((t_token *)(token->content))->type == TOKEN_PIPE)
		{
			result = parse_pipe(token, left, shell);
			if (result.type != PARSE_RESULT_SUCCESS)
				return (result);
			left = result.ast;
			token = result.current_token;
		}
		else
			break ;
	}
	return (parse_success(left, token));
}

/* ================================== END ================================== */
