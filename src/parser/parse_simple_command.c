/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 23:42:07 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/18 23:46:23 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>

// Internal libraries
#include <libft/string.h>
#include <libft/linked_list.h>

// Project headers
#include "structs/s_ast.h"
#include "structs/s_shell.h"
#include "enums/e_redir_type.h"
#include "structs/s_redirection.h"

#include "modules/ast.h"

// Local headers
#include "parse_result.h"
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

	data = ast_command_data(args, redirs);
	if (data == NULL)
		return (0);
	*ast = ast_new_node(AST_COMMAND, data, NULL, NULL);
	if (*ast == NULL)
	{
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
 * @brief The body of the loop of the parse_simple_command function.
 * @note This function is used to comply with 42 Norm's
 * "max 25 line per function" rule. Do not use outside of the
 * parse_simple_command function.
 * @returns The parse_result object of the subsequent function calls.
 * @note The outer loop should break if the parse_result is SUCCESS,
 * but the current_token pointer is NULL.
 */
static t_parse_result	parse_simple_command_loop_body(t_list **token,
	t_list **redirs, t_list **args, t_shell shell)
{
	t_token			*token_obj;
	t_parse_result	redir_result;

	token_obj = (t_token *)((*token)->content);
	if (get_redir_type(token_obj->type) != REDIR_NONE)
	{
		redir_result = parse_redirection(*token, redirs, shell);
		if (redir_result.type != PARSE_RESULT_SUCCESS)
			return (redir_result);
		*token = redir_result.current_token;
		return (parse_success(NULL, *token));
	}
	else if (token_obj->type == TOKEN_WORD)
	{
		if (save_word_to_arg_list(args, token_obj->data.word) != 1)
			return (parse_runtime_error(*token));
	}
	else
		return (parse_success(NULL, NULL));
	*token = (*token)->next;
	return (parse_success(NULL, *token));
}

/**
 * @brief Parses a simple command with arguments and redirections.
 * @returns A struct of t_parse_result.
 * This structs holds the data about the operation.
 */
t_parse_result	parse_simple_command(t_list *tokens, t_shell shell)
{
	t_parse_result	result;
	t_list			*redirs;
	t_list			*args;
	t_list			*old_token;

	result = parse_runtime_error(tokens);
	if (tokens == NULL || tokens->content == NULL)
		return (result);
	redirs = NULL;
	args = NULL;
	while (tokens != NULL && tokens->content != NULL)
	{
		old_token = tokens;
		result = parse_simple_command_loop_body(&tokens, &redirs, &args, shell);
		if (result.type != PARSE_RESULT_SUCCESS)
			return (free_parse_lists(&args, &redirs, result));
		if (tokens == old_token)
			break ;
	}
	if (save_command_ast_node(args, &(result.ast), redirs) != 1)
		return (free_parse_lists(&args, &redirs, parse_runtime_error(tokens)));
	return (parse_success(result.ast, tokens));
}
