/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/24 16:49:53 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <libft/string.h>
#include <libft/predicate.h>

#include "structs/s_ast.h"
#include "modules/ast.h"

#include "expansion_data.h"

/**
 * @brief Takes a list of expansion data blocks and returns the total length of
 * the whole string that will be generated.
 * @note This function is used to correctly allocate memory beforehand.
 */
static size_t	get_new_word_length(t_list *expansion_data)
{
	size_t		len;
	t_expansion	*data;

	len = 0;
	while (expansion_data != NULL && expansion_data->content != NULL)
	{
		data = (t_expansion *)(expansion_data->content);
		if (data->type == EXPANSION_WORD)
			len += data->data.word.end - data->data.word.start;
		else if (data->type == EXPANSION_VARIABLE)
			len += ft_strlen(data->data.variable_value);
		expansion_data = expansion_data->next;
	}
	return (len);
}

/**
 * @brief Takes a list of expansion data blocks,
 * builds the expanded version of a string and returns it.
 */
static char	*get_expanded_word(char *str, t_list *expansion_data)
{
	char		*word;
	size_t		i;
	t_expansion	*data;
	size_t		len;

	len = get_new_word_length(expansion_data) + 1;
	if (len == 1)
		return (NULL);
	word = malloc(sizeof(char) * len);
	if (word == NULL)
		return (NULL);
	i = 0;
	while (expansion_data != NULL && expansion_data->content != NULL)
	{
		data = (t_expansion *)(expansion_data->content);
		if (data->type == EXPANSION_WORD)
		{
			ft_strlcpy(word + i, str + data->data.word.start, data->data.word.end - data->data.word.start + 1);
			i += data->data.word.end - data->data.word.start;
		}
		else if (data->type == EXPANSION_VARIABLE && data->data.variable_value != NULL)
		{
			i += ft_strlcpy(word + i, data->data.variable_value, len - i);
		}
		expansion_data = expansion_data->next;
	}
	return (word);
}

/**
 * @brief Takes a list node, checks if the content needs
 * to be expanded and does so.
 * @return 0 on failure, 1 on success, 2 on NULL node.
 */
static int	expand_word_list(t_list *node, t_list *envp)
{
	char	*word;
	t_list	*data;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		word = (char *)(node->content);
		data = get_expansion_data(word, envp);
		if (data != NULL)
		{
			word = get_expanded_word(word, data);
			ft_lstclear(&data, free);
			free(node->content);
			node->content = (void *)word;
		}
		else if (errno != 0)
			return (0);
		node = node->next;
	}
	return (1);
}

/**
 * @brief Takes an AST node, checks if it's of type AST_COMMAND
 * and then performs expansion on it.
 * @return 0 on failure, 1 on success, 2 on NULL ast parameter.
 */
static int	expand_command_arguments(t_ast *ast)
{
	t_list	*args;

	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
		return (2);
	args = ast->data->ast_command.arguments;
	if (args == NULL)
		return (2);
	if (expand_word_list(args, NULL) != 1)
		return (0);
	return (1);
}

//TODO: Remove
static void	print_command_args(t_ast *ast)
{
	t_list	*lst;

	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
		return ;
	lst = ast->data->ast_command.arguments;
	if (lst == NULL)
		return ;
	printf("CMD: %s, Args: ", (char *)(lst->content));
	while (lst != NULL)
	{
		printf("%s", (char *)(lst->content));
		if (lst->next != NULL)
			printf(", ");
		lst = lst->next;
	}
	printf("\n");
}

/**
 * @brief Takes a syntax tree and expands arguments in it.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_list *env)
{
	(void)env;
	if (root == NULL)
		return (2);
	printf("\n(DEBUG: Starting shell step 3: Expansion)\n");
	printf("(DEBUG: Step 3.1: Variable Substiton)\n");
	if (root->type == AST_COMMAND)
	{
		if (expand_command_arguments(root) != 1)
			return (0);
	}
	if (root->left != NULL)
	{
		if (expansion(root->left, env) != 1)
			return (0);
	}
	if (root->right != NULL)
	{
		if (expansion(root->right, env) != 1)
			return (0);
	}
	printf("(DEBUG: Step 3.2: Printing New Variables)\n");
	ast_traverse_preorder(root, print_command_args);
	return (1);
}
