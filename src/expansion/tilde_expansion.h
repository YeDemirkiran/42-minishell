/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 21:50:50 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/04 22:06:58 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef TILDE_EXPANSION_H
# define TILDE_EXPANSION_H

# include "structs/s_ast.h"
# include "structs/s_shell.h"

/**
 * @brief Takes a syntax tree and expands tilde symbols in it.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	perform_tilde_expansion(t_ast *root, t_envp_list **list);

#endif
