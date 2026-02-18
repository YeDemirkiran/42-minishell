/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 15:30:10 by muayna            #+#    #+#             */
/*   Updated: 2026/02/18 15:11:14 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../../lib/garbage_collector/gb.h"
#include "enums/e_redir_type.h"
#include "fcntl.h"
#include "modules/builtin.h"
#include "modules/executor.h"
#include "modules/signal.h"
#include "modules/utils.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "sys/wait.h"
#include "unistd.h"
#include <errno.h>
#include <libft.h>
#include <minishell_lb.h>

void	find_shell_level(t_shell *shell)
{
	char	*shlvl_value;
	int		value;
	t_list	shlvl;

	shlvl_value = find_value_on_envp(&shell->env->envp_list, "SHLVL");
	if (shlvl_value == NULL)
	{
		shlvl.content = "SHLVL=1";
		shlvl.next = NULL;
		add_variable_on_envplist(&shell->env->envp_list, &shlvl);
		clear_arr(&shell->env->envp_char);
		shell->env->envp_char = envp_list_to_char_arr(shell->env->envp_list);
		return ;
	}
	value = ft_atoi(shlvl_value);
	value++;
	update_envp_value(&shell->env->envp_list, "SHLVL",
		ft_long_term_itoa(value));
	clear_arr(&shell->env->envp_char);
	shell->env->envp_char = envp_list_to_char_arr(shell->env->envp_list);
}
