/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:23:43 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/03 23:01:31 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "structs/s_ast.h"
# include <minishell_lb.h>
/*BUILTIN NAMES*/

typedef struct s_god_struct_envp
{
	t_envp_list *envp_list;
	char		**envp_char;
} t_god_struct_envp;

void	executor(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out);
int isbuiltin(const char *command);
void    run_builtin(t_ast *ast, char ***envp_char, t_envp_list **envp_list);
void run_redirs(t_list **redirs);
char *command_path(t_ast *ast);
char **list_to_char_arr(t_list *list);
int isbuiltin(const char *command);

#endif
