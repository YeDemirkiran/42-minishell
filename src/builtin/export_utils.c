/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 12:29:35 by muayna            #+#    #+#             */
/*   Updated: 2025/12/31 12:29:35 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

static int special_strlen(char *str, char until_char)
{
	int i;

	if(str == NULL || until_char == '\0')
		return 0;
	i = 0;
	while(str[i] && str[i] != until_char)
		i++;
	return i;
}

static void take_id_name(char *ast_input, t_envp_list **envp_list)
{
	int i;
	t_envp_list *tmp;
	int strlen;

	strlen = special_strlen(ast_input, '=');
	tmp = (*envp_list);
	i = 0;
	tmp->id_name = ft_malloc(sizeof(char) * (strlen + 1), 0);
	while(ast_input[i] != '=' && ast_input[i])
	{
		tmp->id_name[i] = ast_input[i];
		i++;
	}
	tmp->id_name[i] = '\0';
}

static void take_id_value(char *ast_input, t_envp_list **envp_list)
{
	int i;
	char *value_tmp_ast;
	t_envp_list *tmp;
	int strlen;

	tmp = (*envp_list);
	i = 0;
	if(ft_strchr(ast_input, '='))
	{
		strlen = ft_strlen((ft_strchr(ast_input, '=') + 1));
		value_tmp_ast = (ft_strchr(ast_input, '=') + 1);
		tmp->value = ft_malloc(strlen + 1, 0);
		while(i < strlen)
		{
			tmp->value[i] = value_tmp_ast[i];
			i++;
		}
		tmp->isvaluempty = 0;
		tmp->value[i] = '\0';
	}
	else
	{
		tmp->isvaluempty = 1;
		tmp->value = NULL;
	}
}

static int is_start_wrong_character(char *str)
{
	int i;

	i = 0;
	if(!ft_isalpha(str[0]))
	{
		printf("-babashell: export: %s: not a valid identifier\n", str);
		return 1;		
	}
	while(str[i] != '=')
	{
		if(!ft_isalpha(str[i]))
		{
			printf("-babashell: export: %s: not a valid identifier\n", str);
			return 1;
		}
		i++;
	}
	return 0;
}

void add_variable_envp_list(t_envp_list **list, t_ast *ast)
{
	t_list *root;
	t_envp_list *tmp;

	root = ast->data->ast_command.arguments->next;
	tmp = (*list);
	while(tmp->next != NULL)
		tmp = tmp->next;
	while(root != NULL)
	{
		if(is_start_wrong_character(((char *)root->content)))
			break ;
		tmp->next = ft_malloc(sizeof(t_envp_list), 0);
		tmp = tmp->next;
		take_id_name(((char *)root->content), &tmp);
		take_id_value(((char *)root->content), &tmp);
		root = root->next;
	}
	tmp->next = NULL;
}
