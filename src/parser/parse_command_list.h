/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_list.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:26:49 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:16:04 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_COMMAND_LIST_H
# define PARSE_COMMAND_LIST_H

# include "structs/s_ast.h"
# include "structs/s_shell.h"

# include "parse_result.h"

/**
 * @brief Main entry point for the parser. Calls the required grammar rules.
 */
t_parse_result	parse_command_list(t_list *tokens, t_shell shell);

#endif
