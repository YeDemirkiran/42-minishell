/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:22:18 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/24 16:45:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <libft/linked_list.h>

#include "modules/ast.h"
#include "structs/s_token.h"
#include "structs/s_ast.h"
#include "structs/s_shell.h"

#include "parse_result.h"
#include "parse_command_list.h"

/**
 * @brief Parses subshells.
 */
t_parse_result	parse_subshell(t_list *token, t_shell shell)
{
	t_parse_result	result;
	t_ast			*root;
	t_token			*token_obj;

	if (token == NULL || token->content == NULL)
		return (parse_runtime_error(token));
	else if (token->next == NULL || token->next->content == NULL)
		return (parse_uncomplete_error(token));
	token_obj = (t_token *)(token->next->content);
	if (token_obj->type == TOKEN_PAREN_RIGHT
		|| token_obj->type == TOKEN_NEWLINE)
		return (parse_uncomplete_error(token));
	result = parse_command_list(token->next, shell);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	else if (result.current_token == NULL || result.current_token->content == NULL)
		return (parse_uncomplete_error(token));
	token_obj = (t_token *)(result.current_token->content);
	if (token_obj->type != TOKEN_PAREN_RIGHT)
		return (parse_syntax_error(token));
	//printf("COMMAND LIST CURRENT TOKEN: %i\n", ((t_token *)(result.current_token->content))->type);
	root = ast_new_node(AST_SUBSHELL, NULL, result.ast, NULL);
	if (root == NULL)
		return (parse_runtime_error(result.current_token));
	result.ast = root;
	result.current_token = result.current_token->next;
	return (result);
}
