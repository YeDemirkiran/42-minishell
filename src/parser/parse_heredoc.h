/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:48:05 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/30 21:07:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef PARSE_HEREDOC_H
# define PARSE_HEREDOC_H

# include "structs/s_redirection.h"

int	parse_heredoc(t_redirection *redir);

#endif
