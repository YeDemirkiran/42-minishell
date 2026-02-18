/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_ast.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:18:12 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 16:13:19 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_AST_H
# define S_AST_H

# include <libft/linked_list.h>

typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_IF,
	AST_OR,
	AST_SUBSHELL
}	t_ast_type;

typedef union u_ast_data
{
	struct s_command
	{
		t_list	*arguments;
		t_list	*redirs;
	}	ast_command;
}	t_ast_data;

typedef struct s_ast	t_ast;

typedef struct s_ast
{
	t_ast_type	type;
	t_ast_data	*data;

	t_ast		*left;
	t_ast		*right;
}	t_ast;

#endif
