/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 19:22:02 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/06 00:42:04 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <libft/linked_list.h>

#include "structs/s_token.h"
#include "types/t_bool.h"

/**
* @brief Creates a token of type and with data.
* @returns NULL on memory allocation failure,
* and a pointer to the new token on success.
*/
t_token	*create_token(t_token_type type, t_token_data data)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->type = type;
	token->data = data;
	return (token);
}

/**
 * @brief Frees a token and its data.
 */
void	free_token(t_token *token)
{
	if (token->type == TOKEN_WORD && token->data.word != NULL)
		free(token->data.word);
	free(token);
}

static void	free_token_void(void *token)
{
	t_token	*token_real;

	token_real = (t_token *)token;
	free_token(token_real);
}

/**
 * @brief Frees a list of tokens.
 */
void	free_tokens(t_list **tokens)
{
	ft_lstclear(tokens, free_token_void);
}

/**
 * @brief Adds the retrieved token to the end of a linked list.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
int	save_token_to_list(t_list **tokens, t_token *token)
{
	t_list	*new_node;

	if (tokens == NULL || token == NULL)
		return (2);
	new_node = ft_lstnew((void *)token);
	if (new_node == NULL)
		return (0);
	ft_lstadd_back(tokens, new_node);
	return (1);
}
