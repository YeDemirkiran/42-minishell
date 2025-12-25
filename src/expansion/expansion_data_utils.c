/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_data_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 20:15:34 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/25 20:33:26 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <libft/predicate.h>
#include <libft/string.h>
#include <libft/linked_list.h>
#include <libft/write.h>

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
int	is_variable_valid(char *word)
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
int	save_expansion_word(t_list **root, size_t word_start, size_t word_end)
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
int	save_expansion_variable(t_list **root, char *key, t_list *envp)
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
 * @return The length of the variable key, or 0 in failure.
 */
size_t	set_variable_key(char *word, char **key_holder)
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
 * @brief Frees the given parameters if they are not NULL,
 * and then appropiate error message when an error occurs during
 * the variable expansion process.
 * @return NULL
 */
void	*on_expansion_data_error(t_list **root, char *key)
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
