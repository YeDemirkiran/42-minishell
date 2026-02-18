/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_shell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:20:17 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 16:13:20 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_SHELL_H
# define S_SHELL_H

# include "modules/executor.h"
# include "structs/s_ast.h"

typedef struct s_shell
{
	t_god_struct_envp	*env;
	char				**args;
	int					argc;
	unsigned char		exit_status;
	int					pid;
	int					interactive;
	unsigned int		root_shell_type;
	t_ast				*root_ast;
}						t_shell;

#endif
