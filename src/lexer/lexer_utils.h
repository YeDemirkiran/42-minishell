/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 11:44:33 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/04 18:31:58 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef LEXER_UTILS_H
# define LEXER_UTILS_H

# include <stddef.h>

# include <structs/s_token.h>
# include <types/t_bool.h>

/**
* @brief Checks whether if a given character is a metacharacter.
* @returns 0 if the character is not a metacharacter, 1 if it is.
*/
t_bool			is_metachar(unsigned char c);

/**
 * @brief Sets the quote char.
*/
void			set_quote(char *quote_char, char quote_target);

/**
 * @brief Returns the token type of a string.
*/
t_token_type	get_token_type(char *string, size_t *token_length);

#endif
