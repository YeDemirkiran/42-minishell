/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:24:16 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/24 03:32:32 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include <unistd.h>
# include <stdio.h>
# include <dirent.h>
#include <structs/s_shell.h>
# include <libft.h>
# include <s_gb.h>
# include <minishell_lb.h>

# include "structs/s_ast.h"

void		unset(t_envp_list **list, t_ast *ast);
/*CD ERROR CHECK*/
int			check_cd_args_count(t_list *args);
int			check_envp_home_path(t_envp_list *list);
int			is_there_path(void *content, int chdir_value);
int			is_oldpwd_set(t_envp_list *list, char *input);
/*CD ERROR CHECK*/
/*EXPORT*/
void		print_list(t_envp_list **list);
int			export(t_envp_list **list, t_ast *ast);
int			add_variable_on_envplist(t_envp_list **list, t_list *command);
char* 		take_variable_value(t_list *arguments);
char* 		take_variable_name(t_list *arguments);
/*EXPORT*/
int			cd(t_envp_list **list, t_ast *ast);
void		pwd(void);
void		env(t_envp_list **list);
int			echo(t_ast *ast);
void		exit_builtin(t_ast *ast, t_shell *shell, int shell_interactive);

typedef struct s_unset_god_list
{
	t_gb		**root_longterm_gb;
	t_gb		*tmp;
	t_gb		*back_tmp;
	t_envp_list	*target_node;
	t_envp_list	*target_node_back;
	t_list		*args;
	t_list		*args_tmp;
}	t_unset_god_list;

#endif
