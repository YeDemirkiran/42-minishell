/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pid_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:35:50 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 15:10:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/ast.h"
#include "modules/signal.h"
#include "modules/utils.h"

void	reset_pid_list(t_pid_list **list)
{
	t_pid_list	*tmp;

	tmp = (*list);
	while (tmp != NULL)
	{
		tmp->pid = 0;
		tmp = tmp->next;
	}
}
/**
 * ? This func. create pid_list for executor
 * ! Example : cat | ls
 * *	If you don't use like this structure,
		cat wait input and ls does not working.
 * *	But if you use this structure, system run 2 command in same time,
		ls working and finished,
		ls output write on terminal. Cat still wait input but ls is finished,
		you can see ls output in terminal.
 */
void	create_pid_list(t_pid_list *root_pid_list, pid_t pid)
{
	if (root_pid_list->pid == 0)
	{
		root_pid_list->pid = pid;
		root_pid_list->next = NULL;
	}
	else
	{
		while (root_pid_list->next != NULL)
			root_pid_list = root_pid_list->next;
		root_pid_list->next = sort_malloc(sizeof(t_pid_list), 0);
		if (root_pid_list->next == NULL)
			return ;
		root_pid_list->next->pid = pid;
		root_pid_list = root_pid_list->next;
		root_pid_list->next = NULL;
	}
}

static void	take_exit_status_value(int *last_status, int *exit_status)
{
	int	sig;

	if (WIFSIGNALED((*exit_status)))
	{
		sig = WTERMSIG((*exit_status));
		print_signal_message(sig, (*exit_status));
		(*last_status) = 128 + sig;
	}
	else
		(*last_status) = WEXITSTATUS((*exit_status));
}

int	wait_for_pipeline(t_shell *shell)
{
	t_pid_list	*tmp_pid_list;
	int			exit_status;
	int			last_status;

	last_status = 0;
	exit_status = 0;
	tmp_pid_list = shell->env->pid_list;
	if (!tmp_pid_list || tmp_pid_list->pid == 0)
		return (shell->exit_status);
	while (tmp_pid_list != NULL)
	{
		if (tmp_pid_list->pid == -1)
		{
			tmp_pid_list = tmp_pid_list->next;
			continue ;
		}
		waitpid(tmp_pid_list->pid, &exit_status, 0);
		take_exit_status_value(&last_status, &exit_status);
		tmp_pid_list = tmp_pid_list->next;
	}
	reset_pid_list(&shell->env->pid_list);
	return (last_status);
}
