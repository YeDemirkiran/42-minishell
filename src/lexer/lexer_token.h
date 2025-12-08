/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:26:32 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/08 17:05:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef LEXER_TOKEN_H
# define LEXER_TOKEN_H

# include <libft/linked_list.h>

# include "structs/s_token.h"

/**
 * @brief Converts a string stream to word token, starting from index.
 * @returns 0 on failure, 1 on success,
 * 2 if any parameter is NULL or START == END.
 * @note The string parameter is automatically freed on failure.
 */
int	save_word_token(t_list **tokens, char *string);

/**
 * @brief Sets the token data according to the token type.
 * @param tokens Used for word token saving if no type is suitable.
 * @param token_addr Pointer to the local token struct that will be filled.
 * @param word The string that will be used as the source of the data.
 * @returns 0 on failure, 1 on success.
 * @note - If the word doesn't fit any token type, it is saved as a word.
 * @note - Currently only checks for redirections.
 */
int	set_token_data(t_list **tokens, t_token *token_addr, char *word);

#endif
