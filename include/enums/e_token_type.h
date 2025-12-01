/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_token_type.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:13:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/01 17:29:34 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef E_TOKEN_TYPE_H
# define E_TOKEN_TYPE_H

// A token is a sequence of characters considered a single unit by the shell.
// It is either a word or an operator.
typedef enum e_token_type
{
	TOKEN_EOF,
	TOKEN_WORD,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_HEREDOC,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_FILE,
	TOKEN_PIPE,
	TOKEN_NEWLINE,
	TOKEN_PAREN_LEFT,
	TOKEN_PAREN_RIGHT,
	TOKEN_IF_AND,
	TOKEN_IF_OR,
}	t_token_type;

#endif
