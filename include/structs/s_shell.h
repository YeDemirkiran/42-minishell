/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_shell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:20:17 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/21 07:57:15 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef S_SHELL_H
# define S_SHELL_H

# include "modules/executor.h"

typedef struct s_shell
{
	t_god_struct_envp	*env;
	char				**args;
	int					argc;
	unsigned char		exit_status;
	int					pid;
	int					interactive;
}	t_shell;

#endif
