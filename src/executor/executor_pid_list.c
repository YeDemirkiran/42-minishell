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

void reset_pid_list(t_pid_list **list)
{
	t_pid_list *tmp;
	
	tmp = (*list);
	while(tmp != NULL)
	{
		tmp->pid = 0;
		tmp = tmp->next;
	}
}
/**
* ? This func. create pid_list for executor
* ! Example : cat | ls
* *	If you don't use like this structure, cat wait input and ls does not working.
* *	But if you use this structure, system run 2 command in same time, ls working and finished, ls output write on terminal. Cat still wait input but ls is finished, you can see ls output in terminal.
*/
void	create_pid_list(t_pid_list *root_pid_list, pid_t pid)
{
	if(root_pid_list->pid == 0)
	{
		root_pid_list->pid = pid;
		root_pid_list->next = NULL;
	}
	else
	{
		while(root_pid_list->next != NULL)
			root_pid_list = root_pid_list->next;
		root_pid_list->next = sort_malloc(sizeof(t_pid_list), 0);
		if(root_pid_list->next == NULL)
			return ;
		root_pid_list->next->pid = pid;
		root_pid_list = root_pid_list->next;
		root_pid_list->next = NULL;
	}
}

int wait_for_pipeline(t_shell *shell)
{
    t_pid_list  *tmp_pid_list;
    int         exit_status;
    int         last_status;
    int         sig;

    last_status = 0;
    exit_status = 0;
    tmp_pid_list = shell->env->pid_list;

    if (!tmp_pid_list || tmp_pid_list->pid == 0)
        return (shell->exit_status);

    while (tmp_pid_list != NULL)
    {
        waitpid(tmp_pid_list->pid, &exit_status, 0);
        if (WIFSIGNALED(exit_status))
        {
            sig = WTERMSIG(exit_status);
            print_signal_message(sig, exit_status);
            last_status = 128 + sig;
        }
        else
            last_status = WEXITSTATUS(exit_status);
            
        tmp_pid_list = tmp_pid_list->next;
    }
    reset_pid_list(&shell->env->pid_list); 
    
    return (last_status);
}
