/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:36:50 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/01 13:44:53 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include "stdio.h"
#include "modules/ast.h"
#include "parse_command.h"

static t_parse_result	parse_pipe(t_list *token, t_ast *left)
{
	t_ast			*node;
	t_parse_result	result;

	if (token->next == NULL || token->next->content == NULL)
	{
		ast_free_tree(left);
		return (parse_uncomplete_error(token));
	}
	result = parse_command(token->next);
	if (result.type != PARSE_RESULT_SUCCESS)
	{
		ast_free_tree(left);
		return (result);
	}
	else if (result.ast == NULL)
	{
		ast_free_tree(left);
		return (parse_syntax_error(token->next));
	}
	token = result.current_token;
	node = ast_new_node(AST_PIPE, NULL, left, result.ast);
	if (node == NULL)
	{
		ast_free_tree(left);
		ast_free_tree(result.ast);
		return (parse_runtime_error(token));
	}
	return (parse_success(node, token));
}

t_parse_result	parse_pipeline(t_list *token)
{
	t_parse_result	result;
	t_ast			*left;

	result = parse_command(token);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	left = result.ast;
	token = result.current_token;
	while (token != NULL && token->content != NULL)
	{
		if (((t_token *)(token->content))->type == TOKEN_PIPE)
		{
			result = parse_pipe(token, left);
			if (result.type != PARSE_RESULT_SUCCESS)
				return (result);
			left = result.ast;
			token = result.current_token;
			// if (token->next == NULL || token->next->content == NULL)
			// {
			// 	ast_free_tree(left);
			// 	return (parse_uncomplete_error(token));
			// }
			// result = parse_command(token->next);
			// if (result.type != PARSE_RESULT_SUCCESS)
			// {
			// 	ast_free_tree(left);
			// 	return (result);
			// }
			// else if (result.ast == NULL)
			// {
			// 	ast_free_tree(left);
			// 	return (parse_syntax_error(token->next));
			// }
			// token = result.current_token;
			// new_node = ast_new_node(AST_PIPE, NULL, left, result.ast);
			// if (new_node == NULL)
			// {
			// 	ast_free_tree(left);
			// 	ast_free_tree(result.ast);
			// 	return (parse_runtime_error(token));
			// }
			// left = new_node;
		}
		else
			break ;
	}
	return (parse_success(left, token));
}
