/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:23:43 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/24 15:35:42 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

#include <minishell_lb.h>
/*BUILTIN NAMES*/

void	executor(t_ast *ast, char **envp, int fd_in, int fd_out);
int isbuiltin(const char *command);
void    run_builtin(t_ast *ast, char ***envp_char, t_envp_list **envp_list);
void run_redirs(t_list **redirs);
char *command_path(t_ast *ast);
char **list_to_char_arr(t_list *list);
int isbuiltin(const char *command);

#endif
