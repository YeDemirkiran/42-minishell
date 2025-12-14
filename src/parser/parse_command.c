/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/14 18:18:28 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_list			*tmp_list;
	char			*tmp_char;

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
			{
				ft_lstclear(&args, free);
				ft_lstclear(&redirs, free_redir_struct);
				return (result);
			}
			tokens = result.current_token;
			continue ;
		}
		else if (token_obj->type == TOKEN_WORD)
		{
			tmp_char = ft_strdup(token_obj->data.word);
			if (tmp_char == NULL)
			{
				ft_lstclear(&args, free);
				ft_lstclear(&redirs, free_redir_struct);
				return (parse_runtime_error(tokens));
			}
			tmp_list = ft_lstnew((void *)tmp_char);
			if (tmp_list == NULL)
			{
				free(tmp_char);
				ft_lstclear(&args, free);
				ft_lstclear(&redirs, free_redir_struct);
				return (parse_runtime_error(tokens));
			}
			ft_lstadd_back(&args, tmp_list);
		}
		else
			break ;
		tokens = tokens->next;
	}

	t_ast_data	*data;
	char		**c_args;

	c_args = NULL;
	if (args != NULL)
	{
		c_args = ft_lst_to_string_arr(args);
		if (c_args == NULL)
		{
			ft_lstclear(&args, free);
			ft_lstclear(&redirs, free_redir_struct);
			return (parse_runtime_error(tokens));
		}
	}	
	data = ast_command_data(c_args, redirs);
	if (data == NULL)
	{
		ft_lstclear(&args, free);
		ft_lstclear(&redirs, free_redir_struct);
		free(c_args);
		return (parse_runtime_error(tokens));
	}
	result.ast = ast_new_node(AST_COMMAND, data, NULL, NULL);
	if (result.ast == NULL)
	{
		ft_lstclear(&args, free);
		ft_lstclear(&redirs, free_redir_struct);
		free(data->ast_command.arguments);
		free(data);
		return (parse_runtime_error(tokens));
	}
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
