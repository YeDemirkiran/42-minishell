/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_command.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:56 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/10 18:29:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_COMMAND_H
# define S_COMMAND_H

# include <structs/s_linked_list.h>

// Struct of a command, to be used by the executor.
typedef struct s_command
{
	char			*command_path;
	t_linked_list	*arguments;

	int				stdin_fd;
	int				stdout_fd;
	int				stderr_fd;
}			t_command;

#endif
