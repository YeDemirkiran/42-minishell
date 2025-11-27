/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_ast.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:18:12 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/27 15:07:16 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef S_AST_H
# define S_AST_H

# include <enums/e_ast_type.h>
# include <structs/s_linked_list.h>

typedef struct s_ast	t_ast;

typedef struct s_ast
{
	t_ast	*left;
	t_ast	*right;
	enum e_ast_type
	{
		AST_COMMAND,
		AST_PIPE
	}	type;
	union u_ast_data
	{
		struct s_command
		{
			char			*command_path;
			char			**arguments;
			t_linked_list	*redirs;
		}	ast_command;
	}	data;
}	t_ast;

#endif
