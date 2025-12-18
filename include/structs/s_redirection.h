/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_redirection.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:10:46 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 15:20:55 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_REDIRECTION_H
# define S_REDIRECTION_H

typedef struct s_redirection
{
	int		target_fd;
	char	*path;
	int		redir_type;
}				t_redirection;

#endif