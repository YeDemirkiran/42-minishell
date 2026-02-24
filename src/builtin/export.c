/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 01:03:27 by muayna            #+#    #+#             */
/*   Updated: 2026/02/18 01:03:27 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell_lb.h>
#include <modules/builtin.h>

static int	cpy_envplst_mlc_cntrl(t_envp_list **new_list, t_envp_list **tmp)
{
	(*new_list)->id_name = sort_term_strdup((*tmp)->id_name);
	(*new_list)->value = sort_term_strdup((*tmp)->value);
	(*new_list)->isvaluempty = (*tmp)->isvaluempty;
	if ((*new_list)->id_name == NULL)
		return (-1);
	if ((*tmp)->next != NULL)
	{
		(*new_list)->next = sort_malloc(sizeof(t_envp_list), 0);
		if ((*new_list)->next == NULL)
			return (-1);
		(*new_list) = (*new_list)->next;
	}
	(*tmp) = (*tmp)->next;
	return (0);
}

static t_envp_list	*copy_envplist(t_envp_list *source)
{
	t_envp_list	*tmp;
	t_envp_list	*new_list;
	t_envp_list	*new_list_root;

	tmp = source;
	if (source == NULL)
		return (NULL);
	new_list = sort_malloc(sizeof(t_envp_list), 0);
	if (new_list == NULL)
		return (NULL);
	new_list_root = new_list;
	while (tmp != NULL)
		if (cpy_envplst_mlc_cntrl(&new_list, &tmp) == -1)
			return (NULL);
	new_list->next = NULL;
	return (new_list_root);
}

int	export(t_envp_list **list, t_ast *ast)
{
	t_list		*command;
	t_envp_list	*copy_list;
	int			return_value;

	return_value = 0;
	command = ast->data->ast_command.arguments;
	if (command->next == NULL)
	{
		copy_list = copy_envplist((*list));
		print_list(&copy_list);
	}
	else
	{
		command = command->next;
		return_value = add_variable_on_envplist(list, command);
	}
	return (return_value);
}
