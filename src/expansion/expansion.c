/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/20 21:02:53 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include <libft/string.h>

#include "structs/s_ast.h"

#include "expansion_block.h"

static char	*get_value_from_env(char *str, char *key)
{
	size_t	i;
	size_t	len;

	char	*value;

	i = 0;
	while (str[i] != '\0' && key[i] != '\0' && str[i] == key[i])
		i++;
	if (key[i] != '\0' || str[i] != '=')
		return (NULL);
	len = 0;
	while (str[len + (i + 1)] != '\0')
		len++;
	value = ft_substr(str, (unsigned int)(i + 1), len);
	return (value);
}

static char	*get_env_var(char *key, t_list *env)
{
	char	*str;
	char	*value;

	if (key == NULL || env == NULL)
		return (NULL);
	value = NULL;
	while (env != NULL)
	{
		if (env->content != NULL)
		{
			str = (char *)(env->content);
			value = get_value_from_env(str, key);
			if (value != NULL)
				break ;
		}
		env = env->next;
	}
	return (value);
}

static t_list	*get_expansion_data(char *word)
{
	t_list	*node;
	t_list	*root;
	size_t	i;
	size_t	word_start;

	word_start = 0;
	i = 0;
	while (word[i] != '\0')
	{
		if (word[i] == '$')
		{
			// peek and check if variable is valid
			// save word
			// save variable key
			word_start = i + 1;
		}
		i++;
	}
}

/**
 * @brief Takes a list node, checks if the content needs
 * to be expanded and does so.
 * @return 0 on failure, 1 on success, 2 on NULL node.
 */
static int	expand_word(t_list *node)
{
	char	*word;
	size_t	i;

	if (node == NULL || node->content == NULL)
		return (2);
	word = (char *)(node->content);
	i = 0;
	while (word[i] != '\0')
	{

	}
	return (1);
}

#include "stdio.h"
/**
 * @brief Takes an AST and expands arguments in it.
 */
void	expansion(t_ast *root, t_list *env)
{
	(void)root;
	(void)env;
	printf("(DEBUG: Starting shell step 3: Expansion)\n");
	printf("(DEBUG: Step 3.1: Variable Substiton)\n");
}
