/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/22 08:21:44 by yademirk         ###   ########.fr       */
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
 * @brief Saves an expansion data block of type WORD
 * to an expansion data list.
 * @return 0 on failure, 1 on success, 2 on NULL node or root parameters.
 */
static int	save_expansion_word(t_list **node, t_list **root,
	size_t word_start, size_t word_end)
{
	t_list		*lst;
	t_expansion	*expansion;

	if (node == NULL || root == NULL || word_start == word_end)
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
		*root = *node;
	}
	else
		(*node)->next = lst;
	return (1);
}

/**
 * @brief Saves an expansion data block of type VARIABLE
 * to an expansion data list.
 * @return 0 on failure, 1 on success, 2 on NULL node or root parameters.
 * @note Calls getenv() function if envp is NULL.
 */
static int	save_expansion_variable(t_list **node, t_list **root, char *key, t_list *envp)
{
	t_list		*lst;
	t_expansion	*expansion;

	if (node == NULL || root == NULL)
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
	if (*node == NULL)
	{
		*node = lst;
		*root = *node;
	}
	else
		(*node)->next = lst;
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
 * @brief Takes a word and returns the expanded version of it.
 */
static t_list	*expand_word(char *word, t_list *envp)
{
	t_list	*root;
	t_list	*node;
	size_t	i;
	size_t	word_start;
	char	*key;

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
				if (save_expansion_word(&node, &root, word_start, i) == 0)
				{
					ft_lstclear(&root, free);
					return (NULL);
				}
				i += set_variable_key(word + (i + 1), &key);
				// save variable key
				if (save_expansion_variable(&node, &root, key, envp) != 1)
				{
					free(key);
					ft_lstclear(&root, free);
					return (NULL);
				}
				free(key);
			}
			word_start = i + 1;
		}
		i++;
	}
	if (i > word_start && word_start > 0)
	{
		if (save_expansion_word(&node, &root, word_start, i) == 0)
		{
			ft_lstclear(&root, free);
			return (NULL);
		}
	}
	return (root);
}

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
 * @brief Takes a list of expansion data blocks and produces a new word.
 */
static char	*produce_new_word(char *str, t_list *expansion_data)
{
	char		*word;
	size_t		i;
	t_expansion	*data;
	size_t		len;

	len = get_new_word_length(expansion_data) + 1;
	printf("\n(new word len: %lu)\n", len - 1);
	word = malloc(sizeof(char) * len);
	if (word == NULL)
		return (NULL);
	i = 0;
	while (expansion_data != NULL && expansion_data->content != NULL)
	{
		data = (t_expansion *)(expansion_data->content);
		if (data->type == EXPANSION_WORD)
		{
			printf("(WORD VAL: %s, I: %lu)\n", ft_substr(str, data->data.word.start, data->data.word.end - data->data.word.start), i);
			ft_strlcpy(word + i, str + data->data.word.start, data->data.word.end - data->data.word.start + 1);
			i += data->data.word.end - data->data.word.start;
		}
		else if (data->type == EXPANSION_VARIABLE)
		{
			printf("(VAR VAL: %s, I: %lu)\n", data->data.variable_value, i);
			i += ft_strlcpy(word + i, data->data.variable_value, len - i);
		}
		expansion_data = expansion_data->next;
	}
	//word[i] = '\0';
	printf("(word: %s)\n", word);
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
		data = expand_word(word, envp);
		if (data != NULL)
		{
			word = produce_new_word(word, data);
			ft_lstclear(&data, free);
			if (word == NULL)
				return (0);
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
