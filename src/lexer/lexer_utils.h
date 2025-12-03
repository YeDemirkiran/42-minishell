/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:44:33 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/02 15:30:14 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef LEXER_UTILS_H
# define LEXER_UTILS_H

# include <stddef.h>

# include <structs/s_token.h>
# include <types/t_bool.h>

t_token	*create_token(t_token_type type, t_token_data data);
t_bool	is_metachar(unsigned char c);

#endif
