/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_redir_type.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:56:30 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/17 12:36:35 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef E_REDIR_TYPE_H
# define E_REDIR_TYPE_H

typedef enum e_redir_type
{
	REDIR_INPUT = 0,
	REDIR_OUTPUT = 1,
	REDIR_HEREDOC = 2,
	REDIR_APPEND = 3
}	t_redir_type;

#endif