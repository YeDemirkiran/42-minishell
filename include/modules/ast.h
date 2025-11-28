/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:44:24 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/28 23:59:17 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef AST_H
# define AST_H

# include <structs/s_ast.h>
# include <stddef.h>

void	ast_traverse_preorder(t_ast *ast, void (*f)(t_ast *));
void	ast_traverse_postorder(t_ast *ast, void (*f)(t_ast *));

#endif
