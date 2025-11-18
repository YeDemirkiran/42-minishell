/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:44:33 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/18 11:49:15 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef LEXER_UTILS_H
# define LEXER_UTILS_H

# include <stddef.h>

# include <enums/e_token_type.h>
# include <structs/s_token.h>

t_token	create_token(t_token_type type, char *data);
int		is_metachar(unsigned char c);
size_t	get_token_count(char *input);

#endif
