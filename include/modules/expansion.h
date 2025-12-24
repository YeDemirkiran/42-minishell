/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:42 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/24 15:44:19 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXPANSION_H
# define EXPANSION_H

#include "structs/s_ast.h"

/**
 * @brief Takes a syntax tree and expands arguments in it.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_list *env);

#endif
