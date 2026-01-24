/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:23:57 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/25 02:45:12 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "structs/s_ast.h"
#include "structs/s_shell.h"

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

	if (tokens == NULL || tokens->content == NULL)
		return (parse_runtime_error(NULL));
	//printf("\ncalling cond\n");
	result = parse_conditional(tokens, shell);
	return (result);
}

#else

/**
 * @brief Main entry point for the parser. Calls the required grammar rules.
 */
t_parse_result	parse_command_list(t_list *tokens, t_shell shell)
{
	t_parse_result	result;

	if (tokens == NULL || tokens->content == NULL)
		return (parse_runtime_error(NULL));
	//printf("\ncalling pipeline\n");
	result = parse_pipeline(tokens, shell);
	return (result);
}

#endif
