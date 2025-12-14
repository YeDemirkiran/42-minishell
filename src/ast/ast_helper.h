/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helper.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:27:34 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/12 18:30:20 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef AST_HELPER_H
# define AST_HELPER_H

# include "structs/s_ast.h"

/**
 * @brief Frees an AST node's data only if it's of type AST_COMMAND.
 */
void	ast_free_command(t_ast *ast);

#endif
