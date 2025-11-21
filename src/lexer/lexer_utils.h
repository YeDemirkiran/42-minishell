/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:44:33 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/21 15:29:56 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_UTILS_H
# define LEXER_UTILS_H

# include <stddef.h>

# include <enums/e_token_type.h>
# include <structs/s_token.h>
# include <types/t_bool.h>

t_token	create_token(t_token_type type, char *data);
t_bool	is_metachar(unsigned char c);
size_t	get_token_count(char *input);

#endif
