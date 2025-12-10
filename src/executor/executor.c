/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2025/12/11 00:21:38 by muayna           ###   ########.fr       */
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

#include <structs/s_ast.h>

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
	(void)ast;
	pid_t pid;

	pid = fork();
	delete_new_line(&ast->data->ast_command.arguments);
	if(pid == 0)
	{
		dup2(fd_in, 0);
		dup2(fd_out, 1);
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
