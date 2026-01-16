/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:24:11 by muayna            #+#    #+#             */
/*   Updated: 2026/01/16 11:01:57 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

static int check_option(char *str)
{
	int i;

	i = 1;
	if(str == NULL)
		return 1;
	if(str[0] == '-')
	{
		i++;
		while(str[i])
		{
			if(str[i] != 'n')
				return 1;
			i++;
		}
	}
	else
		return 1;
	return 0;
}

static void skip_option(t_list **args)
{
	int checker;

	while((*args) != NULL)
	{
		checker = check_option((char *)(*args)->content);
		if(checker == 1)
			return ;
		(*args) = (*args)->next;
	}
}


int echo(t_ast *ast)
{
	t_list *args;
	int checker;

	args = ast->data->ast_command.arguments->next;
	if(args == NULL)
	{
		write(1, "\n", 1);
		return 0;
	}
	checker = check_option((char *)args->content);
	skip_option(&args);
	while(args != NULL)
	{
		write(1, (char *)args->content, ft_strlen((char *)args->content));
		if(args->next != NULL)
			write(1, " ", 1);
		args = args->next;
	}
	if(checker == 1)
		write(1, "\n", 1);
	return 0;
}
