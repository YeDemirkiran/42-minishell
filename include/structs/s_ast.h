/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_ast.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 10:18:12 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/27 16:26:53 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef S_AST_H
# define S_AST_H

# include <structs/s_linked_list.h>

typedef struct s_ast	t_ast;

typedef struct s_ast
{
	t_ast	*left;
	t_ast	*right;
	enum
	{
		AST_COMMAND,
		AST_PIPE
	}		e_type;
	union
	{
		struct s_command
		{
			char			*command_path;
			char			**arguments;
			t_linked_list	*redirs;
		}	ast_command;
	}		u_data;
}	t_ast;

#endif
