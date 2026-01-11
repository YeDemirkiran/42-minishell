/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:23:43 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/11 18:21:28 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "structs/s_ast.h"
# include <minishell_lb.h>
/*BUILTIN NAMES*/

typedef struct s_pid_list
{
	pid_t pid;
	struct s_pid_list *next;
} t_pid_list;

typedef struct s_god_struct_envp
{
	t_envp_list *envp_list;
	char		**envp_char;
	t_pid_list *pid_list;
} t_god_struct_envp;

int	executor(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out);
int isbuiltin(const char *command);
void    run_builtin(t_ast *ast, char ***envp_char, t_envp_list **envp_list);
void run_redirs(t_list **redirs);
char *command_path(t_ast *ast);
char **list_to_char_arr(t_list *list);
int isbuiltin(const char *command);

#endif
