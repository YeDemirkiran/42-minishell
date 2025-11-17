/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_token_type.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:13:25 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/17 12:36:07 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef E_TOKEN_TYPE_H
# define E_TOKEN_TYPE_H

typedef enum e_token_type
{
	TOKEN_EOF,
	TOKEN_CMD,
	TOKEN_STRING,
	TOKEN_REDIR,
	TOKEN_FD,
	TOKEN_FILE,
	TOKEN_PIPE,
	TOKEN_NEWLINE,
}	t_token_type;

#endif