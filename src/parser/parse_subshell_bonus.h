/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell_bonus.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 23:28:36 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/22 23:31:34 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PARSE_SUBSHELL_BONUS_H
# define PARSE_SUBSHELL_BONUS_H

#include <libft/linked_list.h>

#include "structs/s_ast.h"
#include "structs/s_shell.h"

#include "parse_result.h"

/**
 * @brief Parses subshells.
 */
t_parse_result	parse_subshell(t_list *token, t_shell shell);

#endif
