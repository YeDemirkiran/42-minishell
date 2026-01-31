/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 23:37:35 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/31 23:58:39 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Project headers
#include "enums/e_redir_type.h"
#include "structs/s_token.h"

// Local headers
#include "parse_redirection.h"
#include "parse_simple_command.h"

#ifdef MINISHELL_BONUS
# include "parse_subshell_bonus.h"
#endif

/* =============================== FUNCTIONS =============================== */

#ifdef MINISHELL_BONUS

/**
 * @brief Takes a token and parses it into either a simple command or
 * a subshell.
 *
 * The process is decided by whether the first token is either a word,
 * a redirection token, or a left parenthesis token.
 */
t_parse_result	parse_command(t_list *tokens, t_shell shell)
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
		result = parse_simple_command(tokens, shell);
	else if (token->type == TOKEN_PAREN_LEFT)
		result = parse_subshell(tokens, shell);
	else
		result = parse_syntax_error(tokens);
	return (result);
}

#else

/**
 * @brief Takes a token and parses it into either a simple command or
 * a subshell.
 *
 * The process is decided by whether the first token is either a word,
 * a redirection token, or a left parenthesis token.
 */
t_parse_result	parse_command(t_list *tokens, t_shell shell)
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
		result = parse_simple_command(tokens, shell);
	else
		result = parse_syntax_error(tokens);
	return (result);
}

#endif

/* ================================== END ================================== */
