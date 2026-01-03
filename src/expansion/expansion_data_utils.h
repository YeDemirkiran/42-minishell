/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_data_utils.h                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 20:24:31 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/03 22:55:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXPANSION_DATA_UTILS_H
# define EXPANSION_DATA_UTILS_H

# include <stddef.h>

# include <libft/linked_list.h>

# include "structs/s_shell.h"

/**
 * @brief Checks whether a word starting from index 0 abides by the
 * minishell variable rules.
 * @return 0 if the word is not a valid variable, 1 otherwise.
 * @note Variables can only consist of alphanumeric values and the '_' symbol.
 */
int		is_variable_valid(char *word);

/**
 * @brief Saves an expansion data block of type WORD
 * to an expansion data list.
 * @return 0 on failure, 1 on success, 2 on NULL node or root parameters.
 */
int		save_expansion_word(t_list **root, size_t word_start, size_t word_end);

/**
 * @brief Takes a word, saves the variable it holds in key_holder,
 * and then returns the length of the key string.
 * @return The length of the variable key, or 0 in failure.
 */
size_t	set_variable_key(char *word, char **key_holder);

/**
 * @brief Saves an expansion data block of type VARIABLE
 * to an expansion data list.
 * @return 0 on failure, 1 on success, 2 on NULL node or root parameters.
 * @note Calls getenv() function if envp is NULL.
 */
int		save_expansion_variable(t_list **root, char *key, t_shell shell);

/**
 * @brief Frees the given parameters if they are not NULL,
 * and then appropiate error message when an error occurs during
 * the variable expansion process.
 * @return NULL
 */
void	*on_expansion_data_error(t_list **root, char *key);

#endif
