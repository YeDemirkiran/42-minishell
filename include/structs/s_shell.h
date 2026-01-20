/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_shell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:20:17 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/20 14:00:08 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_SHELL_H
# define S_SHELL_H

# include "modules/executor.h"

void	shell_exit(unsigned char status, t_shell shell);

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
