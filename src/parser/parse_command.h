/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:28:45 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 16:27:45 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_COMMAND_H
# define PARSE_COMMAND_H

# include "structs/s_ast.h"

# include "parse_result.h"

t_parse_result	parse_command(t_list *tokens);

#endif
