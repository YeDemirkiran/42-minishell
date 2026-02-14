/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_token.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:11:25 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/14 13:03:14 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef S_TOKEN_H
# define S_TOKEN_H

// All token types required by this shell.
typedef enum e_token_type
{
	TOKEN_NONE,
	TOKEN_WORD,
	TOKEN_NEWLINE,
	TOKEN_PIPE,
	TOKEN_REDIR_INPUT,
	TOKEN_REDIR_OUTPUT,
	TOKEN_REDIR_HEREDOC,
	TOKEN_REDIR_APPEND,
	TOKEN_PAREN_LEFT,
	TOKEN_PAREN_RIGHT,
	TOKEN_SINGLE_AND,
	TOKEN_IF_AND,
	TOKEN_IF_OR
}	t_token_type;

// Tagged union data for tokens.
typedef union u_token_data
{
	char	*word;
	int		number;
}	t_token_data;

// A token is a sequence of characters considered a single unit by the shell.
// It is either a word or an operator.
typedef struct s_token
{
	t_token_type	type;
	t_token_data	data;
}	t_token;

#endif
