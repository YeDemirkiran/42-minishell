/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 19:48:05 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:16:12 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HEREDOC_H
# define PARSE_HEREDOC_H

# include "structs/s_redirection.h"
# include "structs/s_shell.h"

int	parse_heredoc(t_redirection *redir, t_shell shell);

#endif
