/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 13:24:30 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/15 13:27:12 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_EXPANSION_H
# define VARIABLE_EXPANSION_H

# include "structs/s_shell.h"

/**
 * @brief Takes a syntax tree and recursively expands arguments in it.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	perform_variable_expansion(t_ast *root, t_shell shell);

#endif