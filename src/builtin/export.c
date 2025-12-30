/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:19:32 by muayna            #+#    #+#             */
/*   Updated: 2025/12/30 22:31:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

static t_envp_list *sort_for_enpvlist(t_envp_list *list)
{
	t_envp_list *small;
	t_envp_list *tmp;
	int i;

	tmp = list;
	small = tmp;
	while(tmp != NULL)
	{
		i = 0;
		if((small->id_name[i] - tmp->id_name[i]) > 0)
			small = tmp;
		else if((small->id_name[i] - tmp->id_name[i]) == 0)
		{
			while(small->id_name[i] && tmp->id_name[i])
			{
				if((small->id_name[i] - tmp->id_name[i]) > 0)
				{
					small = tmp;
					break ;
				}
				else if(small->id_name[i] - tmp->id_name[i] < 0)
					break ;
				i++;
			}
		}
		tmp = tmp->next;
	}
	return small;
}

static t_envp_list *find_node_back(t_envp_list *source, t_envp_list *find)
{
	t_envp_list *tmp;

	tmp = source;
	while(tmp != NULL && tmp->next != find)
		tmp = tmp->next;
	return tmp;
}

static t_envp_list *envp_list_copy(t_envp_list *list)
{
	t_envp_list *new_list;
	t_envp_list *tmp;
	t_envp_list *new_list_root;

	tmp = list;
	if(list == NULL)
		return NULL;
	new_list = malloc(sizeof(t_envp_list));
	new_list_root = new_list;
	while(tmp != NULL)
	{
		new_list->id_name = ft_strdup(tmp->id_name);
		new_list->value = ft_strdup(tmp->value);
		new_list->isvaluempty = tmp->isvaluempty;
		if(tmp->next != NULL)
		{
			new_list->next = malloc(sizeof(t_envp_list));
			new_list = new_list->next;
		}
		tmp = tmp->next;
	}
	new_list->next = NULL;
	return new_list_root;
}

static void print_variable_on_export(t_envp_list *list)
{
	int i;
	int size_list;
	t_envp_list *sorted;
	t_envp_list *back_of_sorted;
	t_envp_list *root;

	size_list = size_of_envp_list(list);
	root = list;
	i = 0;
	while(i < size_list)
	{
		sorted = sort_for_enpvlist(root);
		back_of_sorted = find_node_back(root, sorted);
		printf("%s=\"%s\"\n", sorted->id_name, sorted->value);
		if(back_of_sorted != NULL && back_of_sorted->next != NULL)
			back_of_sorted->next = sorted->next;
		else
			root = sorted->next;
		free(sorted->id_name);
		free(sorted->value);
		free(sorted);
		i++;
	}
}

void export(t_envp_list **list, t_ast *ast)
{
	t_envp_list *tmp;
	t_envp_list *copy_list;
	t_list *root;

	root = ast->data->ast_command.arguments->next;
	tmp = (*list);
	if(ast->data->ast_command.arguments->next == NULL)
	{
		copy_list = envp_list_copy((*list));
		print_variable_on_export(copy_list);
		printf("DEBUG EXPORT : SUCCES\n");
		return ;
	}
	while(tmp->next != NULL)
		tmp = tmp->next;
	while(root != NULL)
	{
		tmp->next = ft_malloc(sizeof(t_envp_list), 0);
		tmp = tmp->next;
		take_id_name(((char *)root->content), &tmp);
		take_id_value(((char *)root->content), &tmp);
		root = root->next;
	}
	tmp->next = NULL;
	return ;
}
