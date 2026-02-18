/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 10:05:50 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/18 13:34:51 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>
#include <errno.h>

// Internal libraires
#include <libft/string.h>

// Project headers
#include "structs/s_ast.h"
#include "structs/s_redirection.h"

/* =============================== FUNCTIONS =============================== */

static char	*get_tilde_expanded_string(const char *str, const char *home)
{
	char	*path_start;
	char	*word;

	if (home == NULL || str == NULL || *str != '~')
		return (NULL);
	path_start = ft_strchr(str, '/');
	if (path_start == NULL)
	{
		if (str[1] == '\0')
			return (ft_strdup(home));
		return (NULL);
	}
	path_start = ft_strdup(path_start);
	if (path_start == NULL)
		return (NULL);
	word = ft_strjoin(home, path_start);
	free(path_start);
	return (word);
}

int	expand_tilde_word_list(t_list *node, const char *home)
{
	char	*word;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		if (node->content == NULL)
		{
			node = node->next;
			continue ;
		}
		word = get_tilde_expanded_string((char *)(node->content), home);
		if (word != NULL)
		{
			free(node->content);
			node->content = (void *)word;
		}
		else if (errno != 0)
			return (0);
		node = node->next;
	}
	return (1);
}

int	expand_tilde_redir_list(t_list *node, const char *home)
{
	char			*word;
	t_redirection	*redir;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		if (node->content == NULL)
		{
			node = node->next;
			continue ;
		}
		redir = (t_redirection *)node->content;
		word = get_tilde_expanded_string((char *)(redir->path), home);
		if (word != NULL)
		{
			free(redir->path);
			redir->path = word;
		}
		else if (errno != 0)
			return (0);
		node = node->next;
	}
	return (1);
}

/* ================================== END ================================== */
