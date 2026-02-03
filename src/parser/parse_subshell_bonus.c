/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:22:18 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:32:09 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <libft/linked_list.h>

// Project hjeaderts
#include "modules/ast.h"
#include "structs/s_token.h"
#include "structs/s_ast.h"
#include "structs/s_shell.h"

// Local headers
#include "parse_result.h"
#include "parse_command_list.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief The initial checking part of subshell parsing.
 *
 * @return
 * - Runtime error if the token or its content is NULL
 *
 * - Uncomplete error if the next token or its content is NULL
 *
 * - Uncomplete error if the next token's type is either PAREN_RIGHT or a
 * newline (meaning it's an empty or unclosed subshell)
 *
 * - Success if these checks don't fail
 */
static t_parse_result	parse_subshell_initial_check(t_list *token)
{
	t_token			*token_obj;

	if (token == NULL || token->content == NULL)
		return (parse_runtime_error(token));
	else if (token->next == NULL || token->next->content == NULL)
		return (parse_uncomplete_error(token));
	token_obj = (t_token *)(token->next->content);
	if (token_obj->type == TOKEN_PAREN_RIGHT
		|| token_obj->type == TOKEN_NEWLINE)
		return (parse_uncomplete_error(token));
	return (parse_success(NULL, NULL));
}

/**
 * @brief Builds the parent subshell AST and returns it.
 *
 * @return Runtime error on new AST node failure, success if no failure occurs.
 */
static t_parse_result	subshell_prepare_final_result(t_parse_result result)
{
	t_ast			*root;

	root = ast_new_node(AST_SUBSHELL, NULL, result.ast, NULL);
	if (root == NULL)
	{
		ast_free_tree(result.ast);
		return (parse_runtime_error(result.current_token));
	}
	result.ast = root;
	result.current_token = result.current_token->next;
	return (result);
}

/**
 * @brief Parses subshells.
 */
t_parse_result	parse_subshell(t_list *token, t_shell shell)
{
	t_parse_result	result;
	t_token			*token_obj;

	result = parse_subshell_initial_check(token);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	result = parse_command_list(token->next, shell);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	else if (result.current_token == NULL
		|| result.current_token->content == NULL)
	{
		ast_free_tree(result.ast);
		return (parse_uncomplete_error(token));
	}
	token_obj = (t_token *)(result.current_token->content);
	if (token_obj->type != TOKEN_PAREN_RIGHT)
	{
		ast_free_tree(result.ast);
		return (parse_uncomplete_error(token));
	}
	return (subshell_prepare_final_result(result));
}

/* ================================== END ================================== */
