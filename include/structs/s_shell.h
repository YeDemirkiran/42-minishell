/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_shell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/01 21:20:17 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/29 17:16:37 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_SHELL_H
# define S_SHELL_H

# include "modules/executor.h"
#include  "structs/s_ast.h"

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
}	t_shell;

#endif
