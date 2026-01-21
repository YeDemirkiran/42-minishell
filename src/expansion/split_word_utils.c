/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_words_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 10:21:24 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/21 10:24:05 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include <libft/string.h>
#include <libft/linked_list.h>

#include "structs/s_shell.h"
#include "modules/utils.h"

#include "variable_utils.h"

char	*get_ifs(t_shell shell)
{
	static const char	*default_ifs = " \t\n";
	char				*ifs;

	ifs = get_env_var("IFS", shell);
	if (ifs == NULL)
		ifs = ft_strdup(default_ifs);
	return (ifs);
}

static int	save_split_word(char *str, int i, t_list **root)
{
	char	*str_tmp;
	t_list	*node;

	if (str == NULL || root == NULL)
		return (2);
	str_tmp = ft_substr(str, 0, i);
	if (str_tmp == NULL)
	{
		ft_lstclear(root, free);
		return (0);
	}
	node = ft_lstnew((void *)str_tmp);
	if (node == NULL)
	{
		free(str_tmp);
		ft_lstclear(root, free);
		return (0);
	}
	ft_lstadd_back(root, node);
	return (1);
}

/**
 * @brief Splits a string into words according to the IFS variable,
 * and returns it in linked list form.
 * @note IFS is retrieved by getenv. If it doesn't exist, then the default
 * IFS is used.
 * @note The defualt IFS is (space)(tab)(newline).
 * @note If IFS is set but has no characters, word splitting is disabled.
 */
t_list	*split_words(char *str, const char *ifs)
{
	t_list	*root;
	size_t	i;
	char	q;

	if (ft_strlen(ifs) == 0)
		return (NULL);
	i = 0;
	root = NULL;
	q = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && ft_strchr(ifs, str[i]) != NULL)
			i++;
		if (str[i] == '\0')
			break ;
		str += i;
		i = 0;
		while (str[i] != '\0' && (q != 0 || ft_strchr(ifs, str[i]) == NULL))
			set_quote(str[i++], &q);
		if (save_split_word(str, i, &root) == 0)
			return (NULL);
	}
	return (root);
}

int	does_need_split(char *str, const char *ifs)
{
	size_t	i;
	char	quote;

	if (ft_strlen(ifs) == 0)
		return (0);
	i = 0;
	quote = 0;
	while (str[i] != '\0')
	{
		set_quote(str[i], &quote);
		if (str[i] != '\0' && (quote == 0 && ft_strchr(ifs, str[i]) != NULL))
			return (1);
		i++;
	}
	return (0);
}
