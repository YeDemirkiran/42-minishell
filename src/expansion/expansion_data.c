/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:26:42 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/25 16:51:51 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <libft/write.h>
#include <libft/string.h>
#include <libft/predicate.h>
#include <libft/linked_list.h>

#include "expansion_data.h"

/**
 * @brief Extracts the value part from a key-value pair if the keys match.
 * @return Address of the value.
 * @note The returned pointer is not malloc'd, don't try to free it.
 */
static char	*get_value_from_env(char *str, char *key)
{
	size_t	i;

	if (str == NULL || key == NULL)
		return (NULL);
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

/**
 * @brief Takes a word, saves the variable it holds in key_holder,
 * and then returns the length of the key string.
 */
static size_t	set_variable_key(char *word, char **key_holder)
{
	size_t	i;

	if (word == NULL || key_holder == NULL)
		return (0);
	*key_holder = NULL;
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
 * @brief This function is called upon encountering the variable ($) sign
 * on a word.
 *
 * The word parameter must start from after the sign.
 */
// int	on_found_variable_sign(char *word, t_list *envp)
// {
// 	char	*key;

// 	if (is_variable_valid(word) == 1)
// 	{
// 		if (save_expansion_word(&root, word_start, i) == 0)
// 		{
// 			ft_lstclear(&root, free);
// 			perror("minishell");
// 			return (NULL);
// 		}
// 		i += set_variable_key(word + (i + 1), &key);
// 		if (key == NULL)
// 		{
// 			ft_lstclear(&root, free);
// 			perror("minishell");
// 			return (NULL);
// 		}
// 		if (save_expansion_variable(&root, key, envp) != 1)
// 		{
// 			free(key);
// 			ft_lstclear(&root, free);
// 			perror("minishell");
// 			return (NULL);
// 		}
// 		free(key);
// 		word_start = i + 1;
// 	}
// }

static void	*on_expansion_data_error(t_list **root, char *key)
{
	if (root != NULL)
		ft_lstclear(root, free);
	if (key != NULL)
		free(key);
	if (errno != 0)
		perror("minishell");
	else
		ft_putstr_fd("minishell: unknown error during variable expansion\n", 2);
	return (NULL);
}

static void	set_quote(char c, char *quote_addr)
{
	if (c == '\'' && *quote_addr != '"')
	{
		if (*quote_addr == '\'')
			*quote_addr = 0;
		else
			*quote_addr = '\'';
	}
	else if (c == '"' && *quote_addr != '\'')
	{
		if (*quote_addr == '"')
			*quote_addr = 0;
		else
			*quote_addr = '"';
	}
}

/**
 * @brief Takes a word and outputs a linked list of expansion data blocks.
 *
 * These blocks are required for the second step of expansion: Concatenation.
 */
t_list	*get_expansion_data(char *word, t_list *envp)
{
	t_list	*root;
	size_t	i;
	size_t	word_start;
	char	*key;
	char	quote;

	if (word == NULL)
		return (NULL);
	word_start = 0;
	i = 0;
	root = NULL;
	quote = 0;
	while (word[i] != '\0')
	{
		while (word[i] != '\0' && word[i] != '$')
			set_quote(word[i++], &quote);
		if (word[i] == '\0')
			break ;
		if (quote == '\'' || is_variable_valid(word + (i + 1)) == 0)
		{
			i++;
			continue ;
		}
		if (save_expansion_word(&root, word_start, i) == 0)
			return (on_expansion_data_error(&root, NULL));
		i += set_variable_key(word + (i + 1), &key);
		if (key == NULL)
			return (on_expansion_data_error(&root, NULL));
		if (save_expansion_variable(&root, key, envp) != 1)
			return (on_expansion_data_error(&root, key));
		free(key);
		word_start = i + 1;
	}
	if (i > word_start && word_start > 0)
		if (save_expansion_word(&root, word_start, i) == 0)
			return (on_expansion_data_error(&root, NULL));
	return (root);
}
