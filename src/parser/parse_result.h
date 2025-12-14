/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_result.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:00:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 16:27:50 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_RESULT_H
# define PARSE_RESULT_H

# include "structs/s_ast.h"
# include "structs/s_token.h"

/**
 * @brief Enum of various parsing result types.
 */
typedef enum e_parse_result_type
{
	PARSE_RESULT_SUCCESS,
	PARSE_RESULT_TOKEN_ERROR,
	PARSE_RESULT_RUNTIME_ERROR
}	t_parse_result_type;

/**
 * @brief Holds the result of a parsing attempt.
 * @param type The result type
 * @param ast The produced AST node
 * @param current_token The last token that was processed
 */
typedef struct s_parse_result
{
	t_parse_result_type	type;
	t_ast				*ast;
	t_list				*current_token;
}	t_parse_result;

/**
 * @brief Creates a parse_result object with given values.
 */
t_parse_result	parse_result(t_parse_result_type type,
	t_ast *ast, t_list *current_token);

/**
 * @brief Creates a parse_result object of type syntax error.
 */
t_parse_result	parse_syntax_error(t_list *token);

/**
 * @brief Creates a parse_result object of type runtime error.
 */
t_parse_result	parse_runtime_error(t_list *token);

/**
 * @brief Creates a parse_result object of type success.
 */
t_parse_result	parse_success(t_ast *ast, t_list *token);

#endif
