/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 12:01:44 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 12:01:48 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// Internal libraries
#include <readline/readline.h>
#include <libft/string.h>
#include <libft/write.h>
#include <get_next_line.h>

// Project headers
#include "g_sig.h"
#include "structs/s_shell.h"

// Local headers
#include "read_input_utils.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes the current and new inputs, adds newline to the new input
 * joins the old input and the new one, and then replaces the old one
 * with the joined new line.
 * @return NULL on error, the new line on success.
 */
static int
	concatenate_old_and_new_input(char **current_input, char *new_input)
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

/**
 * @brief Decides between primary and secondary prompt
 * depending on whether the value of INPUT is NULL.
 * @return Primary prompt if input is NULL, secondary prompt otherwise.
 */
static const char	*get_prompt(const char *primary_prompt,
	const char *secondary_prompt, char *input)
{
	if (input == NULL)
		return (primary_prompt);
	else
		return (secondary_prompt);
}

/**
 * @brief Reads and retuns a line from STDIN. Trailing newline is stripped
 * from the line.
 *
 * @return Line on success, NULL on failure.
 *
 * @note Checks if 0 is a TTY and decides between GNU Readline and
 * 42 GNL.
 */
char	*get_input_line(const char *prompt, int interactive)
{
	char	*str;
	char	*newl;

	if (interactive == 1)
	{
		str = readline(prompt);
		if (g_sig == SIGINT)
		{
			free(str);
			return (NULL);
		}
	}
	else
	{
		str = get_next_line(STDIN_FILENO);
		if (g_sig == SIGINT)
			return (NULL);
		if (str == NULL)
			return (NULL);
		newl = ft_strrchr(str, '\n');
		if (newl != NULL)
			*newl = '\0';
	}
	return (str);
}

static int	check_new_input(const char *new, char **old,
	char quote, t_shell *shell)
{
	if (new == NULL)
	{
		if (*old == NULL)
			return (0);
		free(*old);
		*old = NULL;
		if (g_sig == SIGINT)
			return (0);
		print_uncompleted_eof_error(quote);
		shell->exit_status = 2;
		return (2);
	}
	return (1);
}

/**
 * @brief Prepares a multi-line input by the user
 * and returns it.
 * @return Multi-line (or single) input on success, NULL on failure.
 */
char	*read_input(const char *primary_prompt, const char *secondary_prompt,
	int interactive, t_shell *shell)
{
	char	*input;
	char	*new;
	char	quote;
	int		res;

	input = NULL;
	quote = 0;
	while (1)
	{
		new = get_input_line(get_prompt(primary_prompt,
					secondary_prompt, input), interactive);
		res = check_new_input(new, &input, quote, shell);
		if (res == 0)
			return (input);
		else if (res == 2)
			continue ;
		if (concatenate_old_and_new_input(&input, new) != 1)
			return (NULL);
		quote = is_input_incomplete(input);
		if (quote == 0)
			break ;
	}
	return (input);
}

/* ================================== END ================================== */
