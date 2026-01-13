/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:44:24 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 09:49:07 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef AST_H
# define AST_H

# include <structs/s_ast.h>
# include <stddef.h>

void		ast_traverse_preorder(t_ast *ast, void (*f)(t_ast *));
void		ast_traverse_postorder(t_ast *ast, void (*f)(t_ast *));

t_ast_data	*ast_command_data(t_list *args, t_list *redirs);
t_ast		*ast_new_node(t_ast_type type, t_ast_data *data,
				t_ast *left, t_ast *right);

void		free_redir_struct(void *redir);
void		ast_free_tree(t_ast *ast);

#endif
