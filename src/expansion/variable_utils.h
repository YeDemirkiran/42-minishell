/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_utils.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 13:36:22 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:36:22 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_UTILS_H
# define VARIABLE_UTILS_H

# include "structs/s_shell.h"

/**
 * @brief Checks whether a word starting from index 0 abides by the
 * minishell variable rules.
 * @return 0 if the word is not a valid variable, 1 otherwise.
 * @note Variables can only consist of alphanumeric values and the '_' symbol.
 */
int		is_variable_valid(char *word);

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
int		is_special_variable(char *word);

/**
 * @brief Returns malloc'd value of a special variable.
 * @return Malloc'd value on success, NULL on failure.
 */
char	*get_special_variable(const char *key, t_shell shell);

/**
 * @brief Searches for a variable key in an environment
 * list and returns its value.
 * @note - Calls getenv() if envp is NULL.
 * @note - The returned pointer is not malloc'd, don't try to free it.
 */
char	*get_env_var(char *key, t_shell shell);

#endif