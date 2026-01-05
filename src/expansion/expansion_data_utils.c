/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_data_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 20:15:34 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/05 13:07:08 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <minishell_lb.h>
#include <libft/predicate.h>
#include <libft/string.h>
#include <libft/linked_list.h>
#include <libft/write.h>
#include <libft/math.h>

#include "structs/s_shell.h"

#include "expansion_data.h"

int	is_special_variable(char *word)
{
	static const char	vars[] = {
		'?', '$', '#',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};
	size_t				i;

	if (word == NULL || word[0] == '\0')
		return (0);
	i = 0;
	while (i < sizeof(vars))
	{
		if (word[0] == vars[i])
			return (1);
		i++;
	}
	return (0);
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
	if (is_special_variable(word) == 1)
		return (1);
	i = 0;
	while (word[i] != '\0' && (ft_isalnum(word[i]) || word[i] == '_'))
		i++;
	if (i == 0)
		return (0);
	return (1);
}

static char	*get_special_variable(const char *key, t_shell shell)
{
	char	*value;
	char	c;

	value = NULL;
	c = key[0];
	if (c == '?')
		value = ft_itoa(shell.last_exit_status);
	else if (c == '$')
		value = ft_itoa(shell.pid);
	else if (c == '#')
		value = ft_itoa(shell.argc - 1);
	else if (ft_isdigit(c) == 1 && (c - '0') < shell.argc)
		value = ft_strdup(shell.args[c - '0']);
	return (value);
}

/**
 * @brief Searches for a variable key in an environment
 * list and returns its value.
 * @note - Calls getenv() if envp is NULL.
 * @note - The returned pointer is not malloc'd, don't try to free it.
 */
static char	*get_env_var(char *key, t_shell shell)
{
	char	*value;

	if (key == NULL)
		return (NULL);
	value = NULL;
	if (is_special_variable(key))
		return (get_special_variable(key, shell));
	else if (shell.env == NULL)
		value = getenv((const char *)key);
	else
		value = find_value_on_envp(&(shell.env->envp_list), key);
	if (value != NULL)
		value = ft_strdup(value);
	return (value);
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
int	save_expansion_variable(t_list **root, char *key, t_shell shell)
{
	t_list		*lst;
	t_expansion	*expansion;

	if (root == NULL || key == NULL)
		return (2);
	expansion = malloc(sizeof(t_expansion));
	if (expansion == NULL)
		return (0);
	expansion->type = EXPANSION_VARIABLE;
	expansion->data.variable_value = get_env_var(key, shell);
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
	if (is_special_variable(word) == 1)
		i = 1;
	else
	{
		while (word[i] != '\0' && (ft_isalnum(word[i]) || word[i] == '_'))
			i++;
	}
	if (i == 0)
		return (0);
	*key_holder = ft_substr(word, (unsigned int)0, i);
	if (*key_holder == NULL)
		return (0);
	return (i);
}

void	free_expansion_data(void *data)
{
	t_expansion	*exp_data;

	if (data == NULL)
		return ;
	exp_data = (t_expansion *)data;
	if (exp_data->type == EXPANSION_VARIABLE)
		free(exp_data->data.variable_value);
	free(exp_data);
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
		ft_lstclear(root, free_expansion_data);
	if (key != NULL)
		free(key);
	if (errno != 0)
		perror("minishell");
	else
		ft_putstr_fd("minishell: unknown error during variable expansion\n", 2);
	return (NULL);
}
