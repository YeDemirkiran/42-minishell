/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/14 13:04:53 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>
#include <errno.h>

// Internal libraries
#include <libft/string.h>
#include <libft/write.h>

// Project headers
#include "enums/e_redir_type.h"
#include "structs/s_ast.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"
#include "g_sig.h"

// Local headers
#include "parse_command_list.h"

/* =============================== FUNCTIONS =============================== */

static char	*get_token_string(t_token *token)
{
	if (token->type == TOKEN_WORD)
		return (token->data.word);
	if (token->type == TOKEN_NEWLINE)
		return ("newline");
	if (token->type == TOKEN_PIPE)
		return ("|");
	if (token->type == TOKEN_SINGLE_AND)
		return ("&");
	if (token->type == TOKEN_IF_AND)
		return ("&&");
	if (token->type == TOKEN_IF_OR)
		return ("||");
	if (token->type == TOKEN_PAREN_LEFT)
		return ("(");
	if (token->type == TOKEN_PAREN_RIGHT)
		return (")");
	if (token->type == TOKEN_REDIR_INPUT)
		return ("<");
	if (token->type == TOKEN_REDIR_OUTPUT)
		return (">");
	if (token->type == TOKEN_REDIR_HEREDOC)
		return ("<<");
	if (token->type == TOKEN_REDIR_APPEND)
		return (">>");
	return (NULL);
}

static void	print_token_error(t_parse_result result)
{
	t_token	*token;
	char	*token_word;

	if (result.type != PARSE_RESULT_TOKEN_ERROR
		&& result.type != PARSE_RESULT_TOKEN_UNCOMPLETE)
		return ;
	if (result.current_token == NULL || result.current_token->content == NULL)
		token = NULL;
	else
		token = (t_token *)(result.current_token->content);
	if (result.type == PARSE_RESULT_TOKEN_ERROR)
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	else if (result.type == PARSE_RESULT_TOKEN_UNCOMPLETE)
		ft_putstr_fd("minishell: uncompleted operator '", 2);
	token_word = get_token_string(token);
	if (token_word == NULL)
		ft_putstr_fd("(null)", 2);
	else
		ft_putstr_fd(token_word, 2);
	ft_putstr_fd("'\n", 2);
}

static void	on_parser_error(t_parse_result result, t_shell *shell)
{
	shell->exit_status = 1;
	if (result.type == PARSE_RESULT_TOKEN_ERROR
		|| result.type == PARSE_RESULT_TOKEN_UNCOMPLETE)
	{
		shell->exit_status = 2;
		print_token_error(result);
	}
	else if (result.type == PARSE_RESULT_RUNTIME_ERROR && errno != 0)
		perror("minishell");
	else
		ft_putendl_fd("minishell: unkown error", 2);
}

/**
 * @brief Takes a list of tokens and parses it as a syntax tree.
 * @returns Full AST on success, NULL on failure.
 * @note Prints the error itself in case of failure.
 */
t_ast	*parse(t_list *tokens, t_shell *shell)
{
	t_ast			*root;
	t_parse_result	result;

	if (tokens == NULL || tokens->content == NULL)
		return (NULL);
	result = parse_command_list(tokens, *shell);
	if (result.type == PARSE_RESULT_ABORT)
	{
		shell->exit_status = (128 + g_sig) * (g_sig > 0);
		return (NULL);
	}
	else if (result.type != PARSE_RESULT_SUCCESS)
	{
		on_parser_error(result, shell);
		return (NULL);
	}
	root = result.ast;
	return (root);
}

/* ================================== END ================================== */
