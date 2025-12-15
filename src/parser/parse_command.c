/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/15 23:35:07 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standrad libraries
#include <stdlib.h>

// Internal libraries
#include <libft/string.h>

// Project headers
#include "enums/e_redir_type.h"
#include "structs/s_ast.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"
#include "modules/ast.h"

// Local headers
#include "parse_redirection.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes a word token and saves it to an arguments list.
 * @note This function is used to comply with 42 Norm's
 * "max 25 line per function" rule. Do not use outside of the
 * parse_simple_command function.
 */
static int	save_word_to_arg_list(t_list **args, char *word)
{
	t_list	*new;
	char	*str;

	str = ft_strdup(word);
	if (str == NULL)
		return (0);
	new = ft_lstnew((void *)str);
	if (new == NULL)
	{
		free(str);
		return (0);
	}
	ft_lstadd_back(args, new);
	return (1);
}

/**
 * @brief Takes an argument and redirections list, creates an AST node and
 * passes it to the given **ast parameter.
 * @note This function is used to comply with 42 Norm's
 * "max 25 line per function" rule. Do not use outside of the
 * parse_simple_command function.
 */
static int	save_command_ast_node(t_list *args, t_ast **ast, t_list *redirs)
{
	t_ast_data	*data;
	char		**c_args;

	c_args = NULL;
	if (args != NULL)
	{
		c_args = ft_lst_to_string_arr(args);
		if (c_args == NULL)
			return (0);
	}
	data = ast_command_data(c_args, redirs);
	if (data == NULL)
	{
		free(c_args);
		return (0);
	}
	*ast = ast_new_node(AST_COMMAND, data, NULL, NULL);
	if (*ast == NULL)
	{
		free(data->ast_command.arguments);
		free(data);
		return (0);
	}
	return (1);
}

/**
 * @brief Static function used for freeing parse lists in the
 * parse_simple_command function.
 *
 * @returns Returns a t_parse_result object with the given parse result type,
 * for convenience.
 *
 * @note This function is used to comply with 42 Norm's
 * "max 25 line per function" rule. Do not use outside of the
 * parse_simple_command function.
 */
static t_parse_result	free_parse_lists(t_list **args, t_list **redirs,
	t_parse_result result)
{
	if (args != NULL)
		ft_lstclear(args, free);
	if (redirs != NULL)
		ft_lstclear(redirs, free_redir_struct);
	return (result);
}

/**
 * @brief Parses a simple command with arguments and redirections.
 * @returns A struct of t_parse_result.
 * This structs holds the data about the operation.
 */
static t_parse_result	parse_simple_command(t_list *tokens)
{
	t_parse_result	result;
	t_token			*token_obj;
	t_list			*redirs;
	t_list			*args;

	result = parse_runtime_error(tokens);
	if (tokens == NULL || tokens->content == NULL)
		return (result);
	redirs = NULL;
	args = NULL;
	while (tokens != NULL && tokens->content != NULL)
	{
		token_obj = (t_token *)(tokens->content);
		if (get_redir_type(token_obj->type) != REDIR_NONE)
		{
			result = parse_redirection(tokens, &redirs);
			if (result.type != PARSE_RESULT_SUCCESS)
				return (free_parse_lists(&args, &redirs, result));
			tokens = result.current_token;
			continue ;
		}
		else if (token_obj->type == TOKEN_WORD)
		{
			if (save_word_to_arg_list(&args, token_obj->data.word) != 1)
				return (free_parse_lists(&args, &redirs,
					parse_runtime_error(tokens)));
		}
		else
			break ;
		tokens = tokens->next;
	}
	if (save_command_ast_node(args, &(result.ast), redirs) != 1)
		return (free_parse_lists(&args, &redirs, parse_runtime_error(tokens)));
	result.type = PARSE_RESULT_SUCCESS;
	ft_lstclear(&args, NULL);
	return (result);
}

/**
 * @brief Parses a subshell.
 */
static t_parse_result	parse_subshell(t_list *tokens)
{
	t_parse_result	result;

	(void)tokens;
	result.type = PARSE_RESULT_RUNTIME_ERROR;
	result.ast = NULL;
	result.current_token = NULL;
	return (result);
}

/**
 * @brief Takes a token and parses it into either a simple command or
 * a subshell.
 *
 * The process is decided by whether the first token is either a word,
 * a redirection token, or a left parenthesis token.
 */
t_parse_result	parse_command(t_list *tokens)
{
	t_parse_result	result;
	t_token			*token;

	if (tokens == NULL || tokens->content == NULL)
		return (parse_runtime_error(NULL));
	token = (t_token *)tokens->content;
	if (token->type == TOKEN_NEWLINE)
		result = parse_success(NULL, tokens->next);
 	else if (token->type == TOKEN_WORD
		|| get_redir_type(token->type) != REDIR_NONE)
		result = parse_simple_command(tokens);
	else if (token->type == TOKEN_PAREN_LEFT)
		result = parse_subshell(tokens);
	else
		result = parse_syntax_error(tokens);
	return (result);
}

/* ================================== END ================================== */
