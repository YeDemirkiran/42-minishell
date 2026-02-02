/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:23:57 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 02:22:25 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "structs/s_ast.h"
#include "structs/s_shell.h"
#include "modules/ast.h"

#include "parse_result.h"

#ifdef MINISHELL_BONUS
# include "parse_conditional_bonus.h"
#else
# include "parse_pipeline.h"
#endif

#ifdef MINISHELL_BONUS

/**
 * @brief Main entry point for the parser. Calls the required grammar rules.
 */
t_parse_result	parse_command_list(t_list *tokens, t_shell shell)
{
	t_parse_result	result;
	t_token			*token;
	static int		level;

	if (tokens == NULL || tokens->content == NULL)
		return (parse_runtime_error(NULL));
	level += 1;
	result = parse_conditional(tokens, shell);
	if (level <= 1 && result.current_token != NULL)
	{
		token = (t_token *)(result.current_token->content);
		if (token->type != TOKEN_NEWLINE)
		{
			level -= 1;
			ast_free_tree(result.ast);
			if (result.type == PARSE_RESULT_SUCCESS)
				result.type = PARSE_RESULT_TOKEN_ERROR;
			return (result);
		}
	}
	level -= 1;
	return (result);
}

#else

/**
 * @brief Main entry point for the parser. Calls the required grammar rules.
 */
t_parse_result	parse_command_list(t_list *tokens, t_shell shell)
{
	t_parse_result	result;
	t_token			*token;

	if (tokens == NULL || tokens->content == NULL)
		return (parse_runtime_error(NULL));
	result = parse_pipeline(tokens, shell);
	if (result.current_token != NULL)
	{
		token = (t_token *)(result.current_token->content);
		if (token->type != TOKEN_NEWLINE)
		{
			ast_free_tree(result.ast);
			if (result.type == PARSE_RESULT_SUCCESS)
				result.type = PARSE_RESULT_TOKEN_ERROR;
			return (result);
		}
	}
	return (result);
}

#endif
