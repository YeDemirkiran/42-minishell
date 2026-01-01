/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_shell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:20:17 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/01 23:25:50 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef SHELL_H
# define SHELL_H

# include "modules/executor.h"

typedef struct s_shell
{
	t_god_struct_envp	*env;
	unsigned char		last_exit_status;
}	t_shell;

#endif
