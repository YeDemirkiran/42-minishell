/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/01/21 08:16:25 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/

#ifndef PARSE_REDIRECTION_H
# define PARSE_REDIRECTION_H

# include "enums/e_redir_type.h"
# include "parse_result.h"
# include "structs/s_shell.h"

int				get_redir_default_fd(t_redir_type type);
t_redir_type	get_redir_type(t_token_type type);
t_parse_result	parse_redirection(t_list *token,
					t_list **redirs, t_shell shell);

#endif
