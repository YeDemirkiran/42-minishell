/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/11 00:42:02 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>

static size_t	count_tokens(char *input)
{
	size_t	token_count;
	int		i;

	i = 0;
	token_count = 0;
	// todo: use ft_isspace
	while (input[i])
	{
		while (input[i] && !(input[i] == ' ' || input[i] == '\t'))
			i++;
		token_count++;
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
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
