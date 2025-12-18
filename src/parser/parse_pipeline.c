/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:36:50 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/17 17:50:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stddef.h>
#include "stdio.h"
#include "modules/ast.h"
#include "parse_command.h"

t_parse_result	parse_pipeline(t_list *token)
{
	t_parse_result	result;
	t_ast			*node_tmp;
	t_ast			*left;
	t_ast			*right;
	t_token			*token_obj;

	result = parse_command(token);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	left = result.ast;
	token = result.current_token;
	while (token != NULL && token->content != NULL)
	{
		token_obj = (t_token *)(token->content);
		if (token_obj->type == TOKEN_PIPE)
		{
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
			right = result.ast;
			node_tmp = ast_new_node(AST_PIPE, NULL, left, right);
			if (node_tmp == NULL)
			{
				ast_free_tree(left);
				return (parse_runtime_error(token));
			}
			left = node_tmp;
		}
		else
			break ;
	}
	return (parse_success(left, token));
}
