/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 15:23:43 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/08 12:05:11 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# define DEFAULT_PATH " "
typedef struct s_shell t_shell; 
typedef struct s_god_struct_envp t_god_struct_envp;

# include <minishell_lb.h>
# include "structs/s_ast.h"
# include "structs/s_shell.h"
/*BUILTIN NAMES*/

typedef struct s_pid_list
{
	pid_t				pid;
	struct s_pid_list	*next;
}	t_pid_list;

typedef struct s_god_struct_envp
{
	t_envp_list	*envp_list;
	char		**envp_char;
	t_pid_list	*pid_list;
}	t_god_struct_envp;


int		executor(t_ast *ast, t_shell *shell, int fd_in, int fd_out);
int		isbuiltin(const char *command);
void	run_builtin(t_ast *ast, t_shell *shell, int print_exit_value);
int		run_redirs(t_list **redirs);
char	*command_path(t_ast *ast, t_shell shell);
char	**list_to_char_arr(t_list *list);
int		isbuiltin(const char *command);
/**/
void	update_special_parameter(t_ast *ast, t_shell *shell);
void    update_bash_value_on_envp(t_shell *shell);
void find_shell_level(t_shell *shell);
#endif
