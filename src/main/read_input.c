/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 14:26:22 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/03 14:29:12 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <errno.h>
#include <readline/readline.h>

#include <libft/string.h>
#include <libft/write.h>

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

/**
 * @brief Checks if an input has unclosed quotes, and returns the type of
 * quote.
 * @return 0 (nul) character if input is complete, and the uncomplete quote
 * character if it's not.
 */
static char	is_input_incomplete(char *input)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (input[i] != '\0')
	{
		set_quote(input[i], &quote);
		i++;
	}
	return (quote);
}

static void	print_uncompleted_eof_error(char quote)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching ", 2);
	if (quote == '"')
		ft_putstr_fd("'\"'", 2);
	else
		ft_putstr_fd("(')", 2);
	ft_putstr_fd("\nminishell: syntax error: unexpected end of file\n", 2);
}

/**
 * @brief Uses GNU Readline, prepares a multi-line input by the user
 * and returns it.
 */
char	*read_input(char *primary_prompt, char *secondary_prompt)
{
	char	*input;
	char	*tmp;
	char	*tmp_2;
	char	*prompt;
	char	quote;

	errno = 0;
	input = NULL;
	quote = 0;
	while (1)
	{
		if (input == NULL)
			prompt = primary_prompt;
		else
			prompt = secondary_prompt;
		tmp = readline(prompt);
		if (tmp == NULL)
		{
			if (input != NULL)
			{
				free(input);
				input = NULL;
				print_uncompleted_eof_error(quote);
				continue ;
			}
			else
				break ;
		}
		tmp_2 = ft_strjoin(tmp, "\n");
		free(tmp);
		if (tmp_2 == NULL)
		{
			if (input != NULL)
				free(input);
			return (NULL);
		}
		tmp = tmp_2;
		if (input == NULL)
			input = tmp;
		else
		{
			tmp_2 = ft_strjoin(input, tmp);
			free(tmp);
			free(input);
			if (tmp_2 == NULL)
				return (NULL);
			input = tmp_2;
		}
		quote = is_input_incomplete(input);
		if (quote == 0)
			break ;
	}
	return (input);
}
