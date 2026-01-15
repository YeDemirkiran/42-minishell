/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:24:11 by muayna            #+#    #+#             */
/*   Updated: 2026/01/15 16:53:56 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

static int check_option_correct(char *str)
{
	int i;

	i = 1;
	if(str[0] != '-')
		return 1;
	while(str[i])
	{
		if(str[i] != 'n')
			return 1;
		i++;
	}
	return 0;
}

static int delete_option(t_list **args, int *isoption)
{
	while((*args) != NULL)
	{
		if(check_option_correct((char *)(*args)->content))
			return 1;
		*isoption = 1;
		(*args) = (*args)->next;
	}
	return 0;
}

static void print_args(char *str, int new_line)
{
	if(new_line == 1)
	{
		write(1, str, ft_strlen(str));
	}
	else
		printf("%s\n", str);
}

int echo(t_ast *ast)
{
	t_list *args;
	int a;

	a = 0;
	args = ast->data->ast_command.arguments;
	args = args->next;
	delete_option(&args, &a);
	while(args != NULL)
	{
		print_args((char *)args->content, a);
		args = args->next;
	}
	return 0;
}
