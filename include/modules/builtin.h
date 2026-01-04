/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:24:16 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/04 16:10:13 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

#include "stdio.h"
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

/*CD ERROR CHECK*/
int check_cd_args_count(t_list *args);
int check_envp_home_path(t_envp_list *list);
int is_there_path(void *content, int chdir_value);
int is_oldpwd_set(t_envp_list *list, char *input);
/*CD ERROR CHECK*/
/*EXPORT*/
void print_list(t_envp_list **list);
void export(t_envp_list **list, t_ast *ast);
t_envp_list *copy_envplist(t_envp_list *source);
/*EXPORT*/
int cd(t_envp_list **list, t_ast *ast);
void	pwd(void);
void	env(t_envp_list **list);

#endif
