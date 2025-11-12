/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/13 01:28:27 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

static unsigned char	ft_isspace(unsigned char c)
{
	return (c == ' ' || c == '\t');
}

static unsigned char	ft_isquote(unsigned char c)
{
	return (c == '\'' || c == '"');
}

static size_t	count_tokens(char *input)
{
	size_t	token_count;
	char	quote_type;
	int		i;

	i = 0;
	token_count = 0;
	while (input[i])
	{
		while (input[i] && !ft_isspace(input[i]) && !ft_isquote(input[i]))
			i++;
		if (ft_isquote(input[i]))
		{
			quote_type = input[i];
			i++;
			while (input[i] && input[i] != quote_type)
				i++;
			i++;
		}
		token_count++;
		while (input[i] && ft_isspace(input[i]))
			i++;
	}
	return (token_count);
}

char	*lexical_analyze(char *input)
{
	size_t	token_count;

	printf("(DEBUG: Starting shell step 1: Lexical Analysis & Tokenization)\n");
	printf("(DEBUG: Step 1: Given input: %s)\n", input);
	if (input == NULL)
		return (NULL);
	token_count = count_tokens(input);
	printf("(DEBUG: Step 1: Lexer token count: %lu)\n", token_count);
	return (NULL);
}
