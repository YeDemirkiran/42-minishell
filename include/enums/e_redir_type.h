/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redir_type.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:56:30 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 16:13:06 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef E_REDIR_TYPE_H
# define E_REDIR_TYPE_H

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_HEREDOC,
	REDIR_APPEND
}	t_redir_type;

#endif
