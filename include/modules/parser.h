/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:23:42 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/07 18:10:06 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PARSER_H
# define PARSER_H

# include "structs/s_ast.h"
# include "structs/s_token.h"
# include "structs/s_shell.h"

t_ast	*parse(t_list *tokens, t_shell *shell);

#endif
