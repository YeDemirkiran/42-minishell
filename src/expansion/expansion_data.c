/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:26:42 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/03 22:55:27 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include "expansion_data.h"
#include "expansion_data_utils.h"

static void	set_quote(char c, char *quote_addr)
{
	if (c == '\'' && *quote_addr != '"')
	{
		if (*quote_addr == '\'')
			*quote_addr = 0;
		else
			*quote_addr = '\'';
	}
	else if (c == '"' && *quote_addr != '\'')
	{
		if (*quote_addr == '"')
			*quote_addr = 0;
		else
			*quote_addr = '"';
	}
}

static void	advance_expansion_loop(char *word, size_t *i, char *quote)
{
	while (word[*i] != '\0' && word[*i] != '$')
	{
		set_quote(word[*i], quote);
		(*i)++;
	}
}

static int	expansion_data_init(size_t *word_start, size_t *i,
	t_list **root, char *quote)
{
	if (word_start == NULL || i == NULL || root == NULL || quote == NULL)
		return (0);
	*word_start = 0;
	*i = 0;
	*root = NULL;
	*quote = 0;
	return (1);
}

static int	init_and_save_variable(t_list **root, t_shell shell,
	char *word, size_t *i)
{
	char	*key;
	int		res;

	key = NULL;
	*i += set_variable_key(word + (*i + 1), &key);
	if (key == NULL)
		return (0);
	res = save_expansion_variable(root, key, shell);
	free(key);
	if (res != 1)
		return (0);
	return (1);
}

/**
 * @brief Takes a word and outputs a linked list of expansion data blocks.
 *
 * These blocks are required for the second step of expansion: Concatenation.
 */
t_list	*get_expansion_data(char *word, t_shell shell)
{
	t_list	*root;
	size_t	i;
	size_t	word_start;
	char	quote;

	if (word == NULL || !expansion_data_init(&word_start, &i, &root, &quote))
		return (NULL);
	while (1)
	{
		advance_expansion_loop(word, &i, &quote);
		if (save_expansion_word(&root, word_start, i) == 0)
			return (on_expansion_data_error(&root, NULL));
		word_start = i;
		if (word[i] == '\0')
			break ;
		if (quote == '\'' || is_variable_valid(word + (i + 1)) == 0)
		{
			i++;
			continue ;
		}
		if (init_and_save_variable(&root, shell, word, &i) != 1)
			return (on_expansion_data_error(&root, NULL));
		word_start = i + 1;
	}
	return (root);
}
