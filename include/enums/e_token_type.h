/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_token_type.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:13:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/30 14:24:40 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef E_TOKEN_TYPE_H
# define E_TOKEN_TYPE_H

typedef enum e_token_type
{
	TOKEN_EOF,
	TOKEN_STRING,
	TOKEN_BLANK,
	TOKEN_REDIR,
	TOKEN_PIPE,
	TOKEN_NEWLINE,
	TOKEN_PARENTHESIS,
	TOKEN_AMPERSAND,
}	t_token_type;

#endif
