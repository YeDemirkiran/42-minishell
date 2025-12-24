/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/24 12:15:25 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include <libft/string.h>
#include <libft/predicate.h>

#include "structs/s_ast.h"
#include "modules/ast.h"

#include "expansion_block.h"

/**
 * @brief Extracts the value part from a key-value pair.
 * @return Address of the value.
 * @note The returned pointer is not malloc'd, don't try to free it.
 */
static char	*get_value_from_env(char *str, char *key)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && key[i] != '\0' && str[i] == key[i])
		i++;
	if (key[i] != '\0' || str[i] != '=')
		return (NULL);
	return (str + (i + 1));
}

/**
 * @brief Searches for a variable key in an environment
 * list and returns its value.
 * @note - Calls getenv() if envp is NULL.
 * @note - The returned pointer is not malloc'd, don't try to free it.
 */
static char	*get_env_var(char *key, t_list *envp)
{
	char	*str;
	char	*value;

	if (key == NULL)
		return (NULL);
	else if (envp == NULL)
		return (getenv((const char *)key));
	value = NULL;
	while (envp != NULL)
	{
		if (envp->content != NULL)
		{
			str = (char *)(envp->content);
			value = get_value_from_env(str, key);
			if (value != NULL)
				break ;
		}
		envp = envp->next;
	}
	return (value);
}

/**
 * @brief Checks whether a word starting from index 0 abides by the
 * minishell variable rules.
 * @return 0 if the word is not a valid variable, 1 otherwise.
 * @note Variables can only consist of alphanumeric values and the '_' symbol.
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
 * @brief Saves an expansion data block of type WORD
 * to an expansion data list.
 * @return 0 on failure, 1 on success, 2 on NULL node or root parameters.
 */
static int	save_expansion_word(t_list **root,
	size_t word_start, size_t word_end)
{
	t_list		*lst;
	t_expansion	*expansion;

	if (root == NULL || word_start == word_end)
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
	ft_lstadd_back(root, lst);
	return (1);
}

/**
 * @brief Saves an expansion data block of type VARIABLE
 * to an expansion data list.
 * @return 0 on failure, 1 on success, 2 on NULL node or root parameters.
 * @note Calls getenv() function if envp is NULL.
 */
static int	save_expansion_variable(t_list **root, char *key, t_list *envp)
{
	t_list		*lst;
	t_expansion	*expansion;

	if (root == NULL || key == NULL)
		return (2);
	expansion = malloc(sizeof(t_expansion));
	if (expansion == NULL)
		return (0);
	expansion->type = EXPANSION_VARIABLE;
	expansion->data.variable_value = get_env_var(key, envp);
	lst = ft_lstnew((void *)expansion);
	if (lst == NULL)
	{
		free(expansion);
		return (0);
	}
	ft_lstadd_back(root, lst);
	return (1);
}

static size_t set_variable_key(char *word, char **key_holder)
{
	size_t	i;

	if (word == NULL || key_holder == NULL)
		return (0);
	i = 0;
	while (word[i] != '\0' && (ft_isalnum(word[i]) || word[i] == '_'))
		i++;
	if (i == 0)
		return (0);
	*key_holder = ft_substr(word, (unsigned int)0, i);
	if (*key_holder == NULL)
		return (0);
	return (i);
}

/**
 * @brief Takes a word and outputs a linked list of expansion data blocks.
 *
 * These blocks are required for the second step of expansion: Concatenation.
 */
static t_list	*get_expansion_data(char *word, t_list *envp)
{
	t_list	*root;
	size_t	i;
	size_t	word_start;
	char	*key;

	word_start = 0;
	i = 0;
	root = NULL;
	while (word[i] != '\0')
	{
		if (word[i] == '$')
		{
			if (is_variable_valid(word + (i + 1)) == 1)
			{
				if (save_expansion_word(&root, word_start, i) == 0)
				{
					ft_lstclear(&root, free);
					return (NULL);
				}
				i += set_variable_key(word + (i + 1), &key);
				if (save_expansion_variable(&root, key, envp) != 1)
				{
					free(key);
					ft_lstclear(&root, free);
					return (NULL);
				}
				free(key);
				word_start = i + 1;
			}
		}
		i++;
	}
	if (i > word_start && word_start > 0)
	{
		if (save_expansion_word(&root, word_start, i) == 0)
		{
			ft_lstclear(&root, free);
			return (NULL);
		}
	}
	return (root);
}

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
		node = node->next;
	}
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
	expand_word_list(args, NULL);
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
 */
void	expansion(t_ast *root, t_list *env)
{
	(void)env;
	if (root == NULL)
		return ;
	printf("\n(DEBUG: Starting shell step 3: Expansion)\n");
	printf("(DEBUG: Step 3.1: Variable Substiton)\n");
	if (root->type == AST_COMMAND)
		expand_command_arguments(root);
	if (root->left != NULL)
		expansion(root->left, env);
	if (root->right != NULL)
		expansion(root->right, env);
	printf("(DEBUG: Step 3.2: Printing New Variables)\n");
	ast_traverse_preorder(root, print_command_args);
}
