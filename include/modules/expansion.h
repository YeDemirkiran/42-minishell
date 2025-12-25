/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:42 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/25 22:08:41 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef EXPANSION_H
# define EXPANSION_H

#include "structs/s_ast.h"

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_list *env);

#endif
