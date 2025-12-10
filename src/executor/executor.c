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
void run_command(t_ast *ast, char **envp)
{
	(void)ast;
	pid_t pid;

	pid = fork();
	delete_new_line(&ast->data->ast_command.arguments);
	if(pid == 0)
	{
		//execve(ft_strjoin("/bin/", cut_wrong_chracter(ast->data->ast_command.command)), ast->data->ast_command.arguments, envp);
		execve(ft_strjoin("/bin/", ast->data->ast_command.command), ast->data->ast_command.arguments, envp);
	}
	pid = wait(NULL);
}

void debug_print(t_ast *ast)
{
	t_ast *tmp;
	int i;

	i = 0;
	tmp = ast; 
	(void)ast;
	printf("(DEBUG: Step 4 AST TYPE  : %d)\n", ast->type);
	while(tmp->data->ast_command.arguments[i] != NULL)
	{
		printf("(DEBUG: Step 4 AST ARGUMENTS : %s)\n", tmp->data->ast_command.arguments[i]);
		i++;
	}
	printf("(DEBUG: Step 4 AST COMMAND  : %s)\n", ast->data->ast_command.command);
}

void	executor(t_ast *ast, char **envp)
{
	if(ast->type == AST_COMMAND)
	{
		run_command(ast, envp);
	}
	debug_print(ast);
}
