/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/22 06:37:02 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <libft/string.h>
#include <libft/predicate.h>

#include "structs/s_ast.h"
#include "modules/ast.h"

#include "expansion_block.h"

// static char	*get_value_from_env(char *str, char *key)
// {
// 	size_t	i;
// 	size_t	len;

// 	char	*value;

// 	i = 0;
// 	while (str[i] != '\0' && key[i] != '\0' && str[i] == key[i])
// 		i++;
// 	if (key[i] != '\0' || str[i] != '=')
// 		return (NULL);
// 	len = 0;
// 	while (str[len + (i + 1)] != '\0')
// 		len++;
// 	value = ft_substr(str, (unsigned int)(i + 1), len);
// 	return (value);
// }

// static char	*get_env_var(char *key, t_list *env)
// {
// 	char	*str;
// 	char	*value;

// 	if (key == NULL || env == NULL)
// 		return (NULL);
// 	value = NULL;
// 	while (env != NULL)
// 	{
// 		if (env->content != NULL)
// 		{
// 			str = (char *)(env->content);
// 			value = get_value_from_env(str, key);
// 			if (value != NULL)
// 				break ;
// 		}
// 		env = env->next;
// 	}
// 	return (value);
// }

/**
 * @brief Checks whether a word starting from index 0 abides by the
 * shell variable rules.
 * @return 0 if the word is not a valid variable, 1 otherwise.
 */
static int	is_variable_valid(char *word)
{
	size_t	i;

	if (word == NULL || word[0] == '\0')
		return (0);
	i = 0;
	while (word[i] != '\0' && (ft_isalnum(word[i]) || word[i] == '_'))
		i++;
	if (i == 0)
		return (0);
	return (1);
}

/**
 * @brief Saves an expansion data block to an expansion data list.
 * @return 0 on failure, 1 on success, 2 on NULL node or root parameters.
 */
static int	save_expansion_word(t_list **node, t_list **root,
	size_t word_start, size_t word_end)
{
	t_list		*lst;
	t_expansion	*expansion;

	if (node == NULL || root == NULL)
		return (2);
	expansion = malloc(sizeof(t_expansion));
	if (expansion == NULL)
		return (0);
	expansion->type = EXPANSION_WORD;
	expansion->data.word.start = word_start;
	expansion->data.word.end = word_end;
	lst = ft_lstnew((void *)expansion);
	if (lst == NULL)
	{
		free(expansion);
		return (0);
	}
	if (*node == NULL)
	{
		*node = lst;
		*root = node;
	}
	else
		(*node)->next = lst;
	return (1);
}

/**
 * @brief Takes a word and returns the expanded version of it.
 */
static t_list	*expand_word(char *word)
{
	t_list	*root;
	t_list	*node;
	size_t	i;
	size_t	word_start;

	word_start = 0;
	i = 0;
	node = NULL;
	root = NULL;
	while (word[i] != '\0')
	{
		if (word[i] == '$')
		{
			// peek and check if variable is valid
			if (is_variable_valid(word + (i + 1)) == 1)
			{
				printf("\n(we have a valid variable)\n");
				// save word
				if (save_expansion_word(&node, &root, word_start, i) != 1)
				{
					ft_lstclear(&root, free);
					return (NULL);
				}
				// save variable key
			}
			word_start = i + 1;
		}
		i++;
	}
	return (root);
}

/**
 * @brief Takes a list node, checks if the content needs
 * to be expanded and does so.
 * @return 0 on failure, 1 on success, 2 on NULL node.
 */
static int	expand_word_list(t_list *node)
{
	char	*word;
	t_list	*data;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		word = (char *)(node->content);
		data = expand_word(word);
		node = node->next;
	}
	if (data == NULL)
		return (1);
	return (1);
}

static void	expand_command_arguments(t_ast *ast)
{
	t_list	*args;

	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
		return ;
	args = ast->data->ast_command.arguments;
	if (args == NULL)
		return ;
	expand_word_list(args);
}

/**
 * @brief Takes an AST and expands arguments in it.
 */
void	expansion(t_ast *root, t_list *env)
{
	(void)root;
	(void)env;
	printf("(DEBUG: Starting shell step 3: Expansion)\n");
	printf("(DEBUG: Step 3.1: Variable Substiton)\n");
	ast_traverse_postorder(root, expand_command_arguments);
}
