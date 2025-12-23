/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:23:43 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/23 20:49:07 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include <minishell_lb.h>
/*BUILTIN NAMES*/

void	executor(t_ast *ast, char **envp, int fd_in, int fd_out);
int isbuiltin(const char *command);
void delete_new_line(char ***s1);
void run_builtin(t_ast *ast, char ***envp_char, t_envp_list **envp_list);

#endif
