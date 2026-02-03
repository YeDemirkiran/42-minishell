/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 16:26:42 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:17:45 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>

// Project headers
#include "modules/utils.h"

// Local headers
#include "variable_utils.h"
#include "expansion_data.h"
#include "expansion_data_utils.h"

/* =============================== FUNCTIONS =============================== */

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

static int	loop_should_continue(char *word,
	char quote, int ignore_quotes)
{
	if (ignore_quotes && quote == '\'')
		return (1);
	else if (is_variable_valid(word) == 0)
		return (1);
	return (0);
}

/**
 * @brief Takes a word and outputs a linked list of expansion data blocks.
 *
 * These blocks are required for the second step of expansion: Concatenation.
 */
t_list	*get_expansion_data(char *word, t_shell shell, int ignore_quotes)
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
		if (loop_should_continue(word + (i + 1), quote, ignore_quotes) == 1)
		{
			i++;
			continue ;
		}
		if (init_and_save_variable(&root, shell, word, &i) != 1)
			return (on_expansion_data_error(&root, NULL));
		word_start = ++i;
	}
	return (root);
}

/* ================================== END ================================== */
