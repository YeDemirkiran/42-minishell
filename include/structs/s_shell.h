/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_shell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:20:17 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/05 17:54:23 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "modules/executor.h"

typedef struct s_shell
{
	t_god_struct_envp	*env;
	char				**args;
	int					argc;
	unsigned char		exit_status;
	int					pid;
}	t_shell;

#endif
