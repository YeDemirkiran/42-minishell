/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:29:05 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/11 23:32:22 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PARSE_REDIRECTION_H
# define PARSE_REDIRECTION_H

# include "enums/e_redir_type.h"
# include "parse_result.h"

t_redir_type	get_redir_type(t_token_type type);
t_parse_result	parse_redirection(t_list *token, t_list **redirs);

#endif
