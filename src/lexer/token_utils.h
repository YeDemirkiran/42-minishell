/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:24:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/03 23:01:41 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef TOKEN_UTILS_H
# define TOKEN_UTILS_H

# include <libft/linked_list.h>

# include "structs/s_token.h"
# include "types/t_bool.h"

/**
* @brief Creates a token of type and with data.
* @returns NULL on memory allocation failure,
* and a pointer to the new token on success.
*/
t_token	*create_token(t_token_type type, t_token_data data);

/**
* @brief Frees a token and its data.
*/
void	free_token(t_token *token);

/**
 * @brief Frees a list of tokens.
 */
void	free_tokens(t_list **tokens);

/**
 * @brief Adds the retrieved token to the end of a linked list.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
t_bool	save_token_to_list(t_list **tokens, t_token *token);

#endif
