/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 23:26:52 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/13 17:15:47 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>

// Internal libraries
#include <libft/string.h>

// Project headers
#include "enums/e_redir_type.h"
#include "structs/s_token.h"
#include "structs/s_redirection.h"
#include "g_sig.h"

// Locla headers
#include "parse_redirection.h"
#include "parse_result.h"
#include "parse_heredoc.h"
#include "parse_heredoc_utils.h"

/* =============================== FUNCTIONS =============================== */

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

static t_parse_result	handle_heredoc(t_redirection *redir, t_shell shell,
	t_list *redirs, t_list *token)
{
	int	heredoc_result;

	if (redir->redir_type == REDIR_HEREDOC)
	{
		heredoc_result = parse_heredoc(redir, shell);
		(void)redirs;
		if (g_sig != 0)
			return (parse_result(PARSE_RESULT_ABORT, NULL, NULL));
		else if (heredoc_result != 1)
			return (parse_runtime_error(token->next));
	}
	return (parse_success(NULL, NULL));
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
	result = handle_heredoc(redirection, shell, *redirs, token);
	if (result.type != PARSE_RESULT_SUCCESS)
		return (result);
	return (parse_success(NULL, token->next->next));
}

/* ================================== END ================================== */
