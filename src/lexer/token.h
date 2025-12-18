/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 17:17:57 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 16:26:01 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include <libft/linked_list.h>
# include "structs/s_token.h"

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
int		save_token_to_list(t_list **tokens, t_token *token);

#endif
