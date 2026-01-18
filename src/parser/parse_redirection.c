/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:26:52 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/19 00:14:30 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include <libft/string.h>

#include "enums/e_redir_type.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"
#include "g_sig.h"

#include "parse_result.h"
#include "parse_heredoc.h"
#include "parse_heredoc_utils.h"

// Returns the default FD value for a redirection.
static int	get_redir_default_fd(t_redir_type type)
{
	int	fd;

	fd = 0;
	if (type == REDIR_INPUT || type == REDIR_HEREDOC)
		fd = 0;
	else if (type == REDIR_OUTPUT || type == REDIR_APPEND)
		fd = 1;
	return (fd);
}

t_redir_type	get_redir_type(t_token_type type)
{
	t_redir_type	redir;

	if (type == TOKEN_REDIR_INPUT)
		redir = REDIR_INPUT;
	else if (type == TOKEN_REDIR_OUTPUT)
		redir = REDIR_OUTPUT;
	else if (type == TOKEN_REDIR_HEREDOC)
		redir = REDIR_HEREDOC;
	else if (type == TOKEN_REDIR_APPEND)
		redir = REDIR_APPEND;
	else
		redir = REDIR_NONE;
	return (redir);
}

static t_parse_result	is_redir_valid(t_list *token)
{
	t_token	*token_obj;

	if (token == NULL || token->content == NULL)
		return (parse_runtime_error(token));
	else if (token->next == NULL || token->next->content == NULL)
		return (parse_runtime_error(token));
	token_obj = (t_token *)(token->next->content);
	if (token_obj->type != TOKEN_WORD)
		return (parse_syntax_error(token->next));
	return (parse_success(NULL, NULL));
}

static t_redirection	*create_redirection(t_list *token)
{
	t_redirection	*redirection;
	t_token			*token_obj;

	redirection = malloc(sizeof(t_redirection));
	if (redirection == NULL)
		return (NULL);
	token_obj = ((t_token *)token->content);
	redirection->redir_type = get_redir_type(token_obj->type);
	if (token_obj->data.number < 0)
		redirection->target_fd = get_redir_default_fd(redirection->redir_type);
	else
		redirection->target_fd = token_obj->data.number;
	token_obj = (t_token *)token->next->content;
	redirection->path = ft_strdup(token_obj->data.word);
	if (redirection->path == NULL)
	{
		free(redirection);
		return (NULL);
	}
	return (redirection);
}

/**
 * @brief Takes a token and saves it in redirections linked list if it's
 * a redirection token.
 */
t_parse_result
	parse_redirection(t_list *token, t_list **redirs, t_shell shell)
{
	t_parse_result	result;
	t_redirection	*redirection;
	t_list			*redir_node;
	int				heredoc_result;

	result = is_redir_valid(token);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	redirection = create_redirection(token);
	if (redirection == NULL)
		return (parse_runtime_error(token));
	redir_node = ft_lstnew((void *)redirection);
	if (redir_node == NULL)
	{
		free(redirection->path);
		free(redirection);
		return (parse_runtime_error(token->next));
	}
	ft_lstadd_back(redirs, redir_node);
	if (redirection->redir_type == REDIR_HEREDOC)
	{
		heredoc_result = parse_heredoc(redirection, shell);
		if (heredoc_result != 1)
			clear_heredoc_tmp_files(*redirs);
		if (g_sig != 0)
			return (parse_result(PARSE_RESULT_ABORT, NULL, NULL));
		else if (heredoc_result != 1)
			return (parse_runtime_error(token->next));
	}
	return (parse_success(NULL, token->next->next));
}
