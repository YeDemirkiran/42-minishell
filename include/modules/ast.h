/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 22:44:24 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/29 16:29:56 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <structs/s_ast.h>
# include <stddef.h>

void	ast_traverse_preorder(t_ast *ast, void (*f)(t_ast *));
void	ast_traverse_postorder(t_ast *ast, void (*f)(t_ast *));

t_ast_data	*ast_command_data(char *command, char **args, t_list *redirs);
t_ast		*ast_new_node(t_ast_type type, t_ast_data data,
				t_ast *left, t_ast *right);
void		ast_free_tree(t_ast *ast);

#endif
