/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:44:24 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/28 22:45:02 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef AST_H
# define AST_H

# include <structs/s_ast.h>
# include <stddef.h>

void	preorder_traverse_ast(t_ast *ast, void (*f)(t_ast *));
void	postorder_traverse_ast(t_ast *ast, void (*f)(t_ast *));

#endif
