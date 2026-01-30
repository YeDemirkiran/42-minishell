/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 15:30:10 by muayna            #+#    #+#             */
/*   Updated: 2026/01/30 18:27:27 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "stdio.h"
#include <libft.h>
#include <errno.h>
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"
#include "sys/wait.h"
#include "signal.h"
#include "../../lib/garbage_collector/gb.h"

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"
#include <minishell_lb.h>
#include "modules/executor.h"
#include "modules/builtin.h"
#include "modules/signal.h"
#include "modules/utils.h"

void find_shell_level(t_shell *shell)
{
	char *shlvl_value;
	int value;

	shlvl_value = find_value_on_envp(&shell->env->envp_list, "SHLVL");
	value = ft_atoi(shlvl_value);
	value++;
	update_envp_value(&shell->env->envp_list, "SHLVL", ft_long_term_itoa(value));
}
