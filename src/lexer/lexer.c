/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/11/13 02:36:16 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

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
	size_t	i;

	i = 0;
	token_count = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		while (input[i])
		{
			while (ft_isquote(input[i]))
			{
				quote_type = input[i];
				i++;
				while (input[i] && input[i] != quote_type)
					i++;
				i++;
			}
			if (ft_isspace(input[i]))
				break ;
			i++;
		}
		token_count++;
	}
	return (token_count);
}

char	*ft_substr(char *str, size_t start, size_t length)
{
	char	*new_str;
	size_t	i;

	// todo: Use garbage collector here
	new_str = malloc((length + 1) * sizeof(char));
	if (new_str == NULL)
		return (NULL);
	i = 0;
	while (i < length)
	{
		new_str[i] = str[start + i];
		i++;
	}
	new_str[i] = '\0';
	//printf("(new substr: %s)\n", new_str);
	return (new_str);
}

static char	**tokenize_input(char *input, size_t token_count)
{
	size_t	i;
	size_t	token_index;
	char	**tokens;
	size_t	current_token_length;
	char	*current_token_start;
	char	quote_type;

	// todo: Use garbage collector here
	tokens = malloc((token_count + 1) * sizeof(char *));
	if (tokens == NULL)
		return (NULL);
	tokens[token_count] = NULL;
	i = 0;
	token_index = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		current_token_length = 0;
		current_token_start = input + i;
		while (input[i])
		{
			while (ft_isquote(input[i]))
			{
				quote_type = input[i];
				i++;
				while (input[i] && input[i] != quote_type)
					i++;
				i++;
			}
			if (ft_isspace(input[i]))
				break ;
			i++;
		}
		current_token_length = (size_t)((input + i) - current_token_start);
		tokens[token_index++] = ft_substr(current_token_start, 0, current_token_length);
		// todo: Null check
	}
	return (tokens);
}

char	**lexical_analyze(char *input)
{
	size_t	token_count;
	char	**tokens;

	printf("(DEBUG: Starting shell step 1: Lexical Analysis & Tokenization)\n");
	printf("(DEBUG: Step 1: Given input: %s)\n", input);
	if (input == NULL)
		return (NULL);
	token_count = count_tokens(input);
	printf("(DEBUG: Step 1.1: Lexer token count: %lu)\n", token_count);
	printf("(DEBUG: Step 1.2: Retrieved tokens: ");
	tokens = tokenize_input(input, token_count);
	int	i = 0;
	if (tokens == NULL)
		return (NULL);
	while (tokens[i] != NULL)
	{
		printf("%s", tokens[i]);
		if (tokens[i + 1] != NULL)
			printf(", ");
		fflush(stdout);
		i++;
	}
	printf(")\n");
	return (tokens);
}
