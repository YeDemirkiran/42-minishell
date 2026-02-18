/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_result.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 14:35:09 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:16:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stddef.h>

// Local headers
#include "parse_result.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Creates a parse_result object with given values.
 */
t_parse_result	parse_result(t_parse_result_type type,
	t_ast *ast, t_list *current_token)
{
	t_parse_result	result;

	result.type = type;
	result.ast = ast;
	result.current_token = current_token;
	return (result);
}

/**
 * @brief Creates a parse_result object of type uncomplete token error.
 */
t_parse_result	parse_uncomplete_error(t_list *token)
{
	return (parse_result(PARSE_RESULT_TOKEN_UNCOMPLETE, NULL, token));
}

/**
 * @brief Creates a parse_result object of type syntax error.
 */
t_parse_result	parse_syntax_error(t_list *token)
{
	return (parse_result(PARSE_RESULT_TOKEN_ERROR, NULL, token));
}

/**
 * @brief Creates a parse_result object of type runtime error.
 */
t_parse_result	parse_runtime_error(t_list *token)
{
	return (parse_result(PARSE_RESULT_RUNTIME_ERROR, NULL, token));
}

/**
 * @brief Creates a parse_result object of type success.
 */
t_parse_result	parse_success(t_ast *ast, t_list *token)
{
	return (parse_result(PARSE_RESULT_SUCCESS, ast, token));
}

/* ================================== END ================================== */
