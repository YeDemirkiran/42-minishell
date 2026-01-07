/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 14:26:22 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/07 22:13:11 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>

#include <libft/string.h>
#include <libft/write.h>
#include <get_next_line.h>

#include "read_input_utils.h"

static int	concatenate_old_and_new_input(char **current_input, char *new_input)
{
	char	*newline_input;

	newline_input = ft_strjoin(new_input, "\n");
	free(new_input);
	if (newline_input == NULL)
	{
		if (*current_input != NULL)
			free(*current_input);
		return (0);
	}
	if (*current_input == NULL)
		*current_input = newline_input;
	else
	{
		new_input = ft_strjoin(*current_input, newline_input);
		free(newline_input);
		free(*current_input);
		if (new_input == NULL)
			return (0);
		*current_input = new_input;
	}
	return (1);
}

static const char	*get_prompt(const char *primary_prompt,
	const char *secondary_prompt, char *input)
{
	if (input == NULL)
		return (primary_prompt);
	else
		return (secondary_prompt);
}

/**
 * @brief Checks if 0 is a TTY and decides between GNU Readline and
 * 42 GNL.
 */
char	*get_input_line(const char *prompt, int interactive)
{
	char	*str;
	char	*newl;

	if (interactive == 1)
		str = readline(prompt);
	else
	{
		str = get_next_line(STDIN_FILENO);
		if (str == NULL)
			return (NULL);
		newl = ft_strrchr(str, '\n');
		if (newl != NULL)
			*newl = '\0';
	}
	return (str);
}

/**
 * @brief Uses GNU Readline, prepares a multi-line input by the user
 * and returns it.
 */
char	*read_input(const char *primary_prompt, const char *secondary_prompt,
	int interactive)
{
	char	*input;
	char	*new;
	char	quote;

	input = NULL;
	while (1)
	{
		new = get_input_line(get_prompt(primary_prompt,
					secondary_prompt, input), interactive);
		if (new == NULL)
		{
			if (input == NULL)
				break ;
			free(input);
			input = NULL;
			print_uncompleted_eof_error(quote);
			continue ;
		}
		if (concatenate_old_and_new_input(&input, new) != 1)
			return (NULL);
		quote = is_input_incomplete(input);
		if (quote == 0)
			break ;
	}
	return (input);
}
