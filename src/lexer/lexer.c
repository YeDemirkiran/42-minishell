/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 14:34:58 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/03 12:45:49 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libft/predicate.h>
#include <libft/string.h>
#include <libft/linked_list.h>

#include "enums/e_metachar.h"
#include "structs/s_token.h"
#include "lexer_utils.h"

/**
 * @brief Frees a token and its data.
 */
static void	free_token(t_token *token)
{
	if (token->type == TOKEN_WORD && token->data.word != NULL)
		free(token->data.word);
	free(token);
}

/**
 * @brief Adds the retrieved token to the end of a linked list.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
static t_bool	save_token_to_list(t_list **tokens, t_token *token)
{
	t_list	*new_node;

	if (tokens == NULL || token == NULL)
		return (2);
	new_node = ft_lstnew((void *)token);
	if (new_node == NULL)
		return (0);
	ft_lstadd_back(tokens, new_node);
	return (1);
}

static t_token_type	get_token_type(char *input, size_t *index)
{
	t_token_type	type;
	size_t			i;

	i = *index;
	while (input[i] != '\0' && !is_metachar(input[i]))
		i++;
	if (i == *index)
		type = TOKEN_WORD;
	else if (ft_strncmp((const)(input + i), "<", i) == 0)
		type = TOKEN_REDIR_INPUT;
	else if (ft_strncmp((const)(input + i), ">", i) == 0)
		type = TOKEN_REDIR_OUTPUT;
	else if (ft_strncmp((const)(input + i), "<<", i) == 0)
		type = TOKEN_REDIR_HEREDOC;
	else if (ft_strncmp((const)(input + i), ">>", i) == 0)
		type = TOKEN_REDIR_APPEND;
	else if (ft_strncmp((const)(input + i), "|", i) == 0)
		type = TOKEN_PIPE;
	else if (ft_strncmp((const)(input + i), "(", i) == 0)
		type = TOKEN_PAREN_LEFT;
	else if (ft_strncmp((const)(input + i), ")", i) == 0)
		type = TOKEN_PAREN_RIGHT;
	else if (ft_strncmp((const)(input + i), "&&", i) == 0)
		type = TOKEN_IF_AND;
	else if (ft_strncmp((const)(input + i), "||", i) == 0)
		type = TOKEN_IF_OR;
	else if (ft_strncmp((const)(input + i), "\n", i) == 0)
		type = TOKEN_NEWLINE;
	*index = i;
	return (type);
}

static t_token_data	get_token_data(char *input, t_token_type type)
{
	t_token_data	data;

	data.word = NULL;
	return (data);
}

/**
 * @brief Converts an input stream to token, starting from index.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
static t_bool	save_metachar_token(t_list **tokens, char *input, size_t *index)
{
	t_token			*token;
	t_token_type	type;
	t_token_data	data;
	size_t			start;

	if (tokens == NULL || input == NULL || index == NULL)
		return (2);
	start = *index;
	type = get_token_type(input, *index);
	data = get_token_data(input, type);
	token = create_token(type, data);
	if (token == NULL)
		return (0);
	if (save_token_to_list(tokens, token) != 1)
	{
		free_token(token);
		return (0);
	}
	return (1);
}

/**
 * @brief Converts an input stream to word token, starting from index.
 * @returns 0 on failure, 1 on success, 2 if any parameter is NULL.
 */
static t_bool	save_word_token(t_list **tokens,
	char *start, char *end)
{
	t_token			*token;
	t_token_data	data;

	if (tokens == NULL || start == end)
		return (2);
	data.word = ft_substr(start, 0, (size_t)(end - start));
	if (data.word == NULL)
		return (0);
	token = create_token(TOKEN_WORD, data);
	if (token == NULL)
	{
		free(data.word);
		return (0);
	}
	if (save_token_to_list(tokens, token) != 1)
	{
		free_token(token);
		return (0);
	}
	return (1);
}

static void	handle_quote(char *quote_char, char quote)
{
	if (quote_char == NULL)
		return ;
	if (*quote_char == '\0')
		*quote_char = quote;
	else if (*quote_char == quote)
		*quote_char = '\0';
}

// Returns tokens from a given string.
//
// Returns NULL on failure.
static t_list	*get_tokens(char *str)
{
	t_list	*tokens;
	size_t	i;
	size_t	t_start;
	char	quote;

	tokens = NULL;
	i = 0;
	t_start = 0;
	quote = '\0';
	while (str[i] != '\0')
	{
		if (str[i] == '"' || str[i] == '\'')
			handle_quote(&quote, str[i]);
		if (quote == '\0' && is_metachar(str[i]))
		{
			save_word_token(tokens, str + t_start, str + i);
			save_metachar_token(tokens, str, &i);
			t_start = i + 1;
		}
		i++;
	}
	save_word_token(tokens, str + t_start, str + i);
	return (tokens);
}

// TODO: Remove this function
static void	debug_print_tokens(t_list *tokens)
{
	t_token	*token;

	while (tokens != NULL)
	{
		token = (t_token *)(tokens->content);
		if (token->type == TOKEN_REDIR_INPUT)
			printf("[REDIR_INPUT]");
		else if (token->type == TOKEN_REDIR_OUTPUT)
			printf("[REDIR_OUTPUT]");
		else if (token->type == TOKEN_REDIR_APPEND)
			printf("[REDIR_HEREDOC]");
		else if (token->type == TOKEN_REDIR_APPEND)
			printf("[REDIR_APPEND]");
		else if (token->type == TOKEN_PIPE)
			printf("[PIPE]");
		else if (token->type == TOKEN_PAREN_LEFT)
			printf("[PAREN_LEFT]");
		else if (token->type == TOKEN_PAREN_RIGHT)
			printf("[PAREN_RIGHT]");
		else if (token->type == TOKEN_IF_AND)
			printf("[IF_AND]");
		else if (token->type == TOKEN_IF_OR)
			printf("[IF_OR]");
		else if (token->type == TOKEN_NEWLINE)
			printf("[NEWLINE]");
		else if (token->type == TOKEN_WORD)
			printf("[TOKEN_WORD, %s]", token->data.word);
		if (tokens->next != NULL)
			printf(", ");
		tokens = tokens->next;
	}
}

// Performs lexical analysis of an input and returns an array of tokens.
// TODO: Remove debug print statements
t_list	*lexical_analyze(char *input)
{
	t_list	*tokens;

	printf("(DEBUG: Starting shell step 1: Lexical Analysis & Tokenization)\n");
	printf("(DEBUG: Step 1: Given input: %s)\n", input);
	if (input == NULL)
		return (NULL);
	printf("(DEBUG: Step 1.1: Retrieved tokens: ");
	tokens = get_tokens(input);
	if (tokens == NULL)
	{
		printf("NULL)\n");
		return (NULL);
	}
	debug_print_tokens(tokens);
	printf(")\n");
	printf("(DEBUG: Step 1 'Lexical Analysis & Tokenization' completed.)\n");
	return (tokens);
}
