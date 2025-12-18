/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2025/12/14 15:25:22 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h" 
#include <libft.h>
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"
#include "sys/wait.h"
#include "../../lib/garbage_collector/gb.h"

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"


/*This function cut \n and \r in char* of end of the line */
char  *cut_wrong_chracter(const char *s1)
{
    int i;
    char *new_str;

    i = 0;
    if (s1 == NULL || !ft_strchr(s1, '\n'))
        return (char *)s1;
    while (s1[i] && (s1[i] != '\n' && s1[i] != '\r'))
        i++;
    new_str = ft_malloc(i + 1, 0);
    i = 0;
    while(s1[i] && (s1[i]!= '\n' && s1[i] != '\r'))
    {
        new_str[i] = s1[i];
        i++;
    }
    new_str[i] = '\0';
    return new_str;
}
void delete_new_line(char ***s1)
{
	char **str;
	int i;

	str = (*s1);
	i = 0;
	while(str[i] != NULL)
	{
		str[i] = cut_wrong_chracter(str[i]);
		i++;
	}
}
void run_command(t_ast *ast, char **envp, int fd_in, int fd_out)
{
	pid_t pid;
	t_redirection	*redir;

	
	pid = fork();
	delete_new_line(&ast->data->ast_command.arguments);
	if(pid == 0)
	{
		dup2(fd_in, 0);
		dup2(fd_out, 1);
		t_list *redirs = ast->data->ast_command.redirs;
		printf("REDIRS NULL: %i\n", redirs == NULL);
		while (redirs != NULL)
		{
			int fd = -1;
			redir = (t_redirection *)redirs->content;
			if (redir->redir_type == REDIR_INPUT)
				fd = open(redir->path, O_RDONLY);
			else if (redir->redir_type == REDIR_OUTPUT)
				fd = open(redir->path, O_WRONLY | O_CREAT, 0777);
			if (fd >= 0)
			{
				dup2(fd, redir->target_fd);
			}		
			redirs = redirs->next;
		}
		execve(ft_strjoin("/bin/", cut_wrong_chracter(ast->data->ast_command.command)), ast->data->ast_command.arguments, envp);
		exit(1);
	}
	wait(NULL);
}

void debug_print(t_ast *ast)
{
	printf("DEBUG ROOT LEG TYPE : %d\n", ast->type);
	printf("DEBUG LEFT LEG TYPE : %d\n", ast->left->type);
	printf("DEBUG LEFT LEG COMMAND : %s\n", ast->left->data->ast_command.command);
	printf("DEBUG RIHT LEG TYPE : %d\n", ast->right->type);
	printf("DEBUG RIHT LEG COMMAND : %s\n", ast->right->data->ast_command.command);
	printf("DEBUG RIHT LEG ARGUMENT : %s\n", ast->right->data->ast_command.arguments[1]);
}

void	executor(t_ast *ast, char **envp, int fd_in, int fd_out)
{
	int pipefd[2];
	
	if(ast->type == AST_COMMAND)
		run_command(ast, envp, fd_in, fd_out);
	else if (ast->type == AST_PIPE)
	{
		pipe(pipefd);
		executor(ast->left, envp, fd_in, pipefd[1]);
		close(pipefd[1]);
		executor(ast->right, envp, pipefd[0], fd_out);
		close(pipefd[0]);
	}
	(void)envp;
	//debug_print(for_debug);
}
