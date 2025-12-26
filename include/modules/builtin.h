/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:24:16 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/26 19:20:44 by muayna           ###   ########.fr       */
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
/*CD ERROR CHECK*/
int cd(t_envp_list **list, t_ast *ast);
void	pwd(t_envp_list **list);
void	env(t_envp_list **list);

#endif
