/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:42 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 09:49:51 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXPANSION_H
# define EXPANSION_H

# include "structs/s_ast.h"
# include "structs/s_shell.h"

/**
 * @brief Takes a string and removes outer-level quotes from it.
 * @note Quotes inside quotes are preserved.
 */
void	remove_quotes(char *str);

/**
 * @brief Takes a string, performs expansion on it if it's needed and
 * replaces the old one with the new one.
 */
int		expand_and_replace_string(char **str_ptr, t_shell shell);

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int		expansion(t_ast *root, t_shell shell);

#endif
