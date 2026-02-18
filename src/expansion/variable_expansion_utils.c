/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 09:54:53 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:34:55 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>

// Internal libraries
#include <libft/linked_list.h>
#include <libft/string.h>

// Local headers
#include "expansion_data.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes a list of expansion data blocks and returns the total length of
 * the whole string that will be generated.
 * @note This function is used to correctly allocate memory beforehand.
 */
static size_t	get_new_word_length(t_list *expansion_data)
{
	size_t		len;
	t_expansion	*data;

	len = 0;
	while (expansion_data != NULL && expansion_data->content != NULL)
	{
		data = (t_expansion *)(expansion_data->content);
		if (data->type == EXPANSION_WORD)
			len += data->data.word.end - data->data.word.start;
		else if (data->type == EXPANSION_VARIABLE)
			len += ft_strlen(data->data.variable_value);
		expansion_data = expansion_data->next;
	}
	return (len);
}

/**
 * @brief Helper function to initialize the get_expanded_word function.
 * @return 0 on failure, 1 on success.
 */
static int	get_expanded_word_init(size_t *len, char **word,
	t_list *expansion_data)
{
	*len = get_new_word_length(expansion_data) + 1;
	if (*len == 1)
		return (0);
	*word = malloc(sizeof(char) * *len);
	if (*word == NULL)
		return (0);
	return (1);
}

/**
 * @brief Takes a list of expansion data blocks,
 * builds the expanded version of a string and returns it.
 */
char	*get_expanded_word(char *str, t_list *expansion_data)
{
	char		*word;
	size_t		i;
	t_expansion	*data;
	size_t		len;

	if (get_expanded_word_init(&len, &word, expansion_data) != 1)
		return (NULL);
	i = 0;
	while (expansion_data != NULL && expansion_data->content != NULL)
	{
		data = (t_expansion *)(expansion_data->content);
		if (data->type == EXPANSION_WORD)
		{
			ft_strlcpy(word + i, str + data->data.word.start,
				data->data.word.end - data->data.word.start + 1);
			i += data->data.word.end - data->data.word.start;
		}
		else if (data->type == EXPANSION_VARIABLE
			&& data->data.variable_value != NULL)
			i += ft_strlcpy(word + i, data->data.variable_value, len - i);
		expansion_data = expansion_data->next;
	}
	return (word);
}

/* ================================== END ================================== */
