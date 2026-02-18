/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 23:45:21 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:16:20 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_SIMPLE_COMMAND_H
# define PARSE_SIMPLE_COMMAND_H

# include <libft/linked_list.h>

# include "structs/s_shell.h"

# include "parse_result.h"

/**
 * @brief Parses a simple command with arguments and redirections.
 * @returns A struct of t_parse_result.
 * This structs holds the data about the operation.
 */
t_parse_result	parse_simple_command(t_list *tokens, t_shell shell);

#endif
