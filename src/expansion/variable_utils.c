/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 14:00:08 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/15 16:44:18 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "structs/s_shell.h"

/**
 * @brief Checks if the given parameter is a special, or a positional
 * argument. These include:
 * 
 * - $$: Get PID
 * 
 * - $?: Get the exit code of the last executed command
 * 
 * - $#: Number of positional arguments
 * 
 * - $0...$9: Positional arguments from 0 to 9
 * 
 * @return 0 if not special variable, 1 if it is.
 */
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

/**
 * @brief Returns malloc'd value of a special variable.
 * @return Malloc'd value on success, NULL on failure.
 */
char	*get_special_variable(const char *key, t_shell shell)
{
	char	*value;
	char	c;

	value = NULL;
	c = key[0];
	if (c == '?')
		value = ft_itoa(shell.exit_status);
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
char	*get_env_var(char *key, t_shell shell)
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
