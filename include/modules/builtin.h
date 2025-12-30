/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:24:16 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/30 22:30:59 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
void export(t_envp_list **list, t_ast *ast);
int cd(t_envp_list **list, t_ast *ast);
void	pwd(void);
void	env(t_envp_list **list);

#endif
