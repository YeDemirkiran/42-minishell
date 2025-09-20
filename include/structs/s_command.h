/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_command.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:56 by yademirk          #+#    #+#             */
/*   Updated: 2025/09/20 23:56:03 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef S_COMMAND_H
# define S_COMMAND_H

# include <structs/s_linked_list.h>

/**
 * @brief Struct of a command, to be used by the executor.
 */

typedef struct s_command
{
	char			*command_path;
	t_linked_list	*arguments;

	int		stdin_fd;
	int		stdout_fd;
	int		stderr_fd;
}			t_command;

#endif
